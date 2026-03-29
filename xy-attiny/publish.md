# TELESCOPE MOTOR CONTROLLER
## A Smart Add-On for the TEC-1

*Control two DC motors from your TEC-1 — using the serial port you already have.*

---

Here is a project that shows just how clever a small chip can be.

The ATtiny85 is a tiny 8-pin microcontroller with built-in PWM outputs. In this project it acts as a motor controller for a telescope — handling the X and Y axes independently via a standard H-bridge chip.

The clever part is how it connects to your TEC-1. There is no extra serial port, no extra wiring, and no changes to the TEC-1 at all. You simply tap into the existing console TX line with a T-junction. The ATtiny listens to everything that goes past on that wire and picks out the motor commands. Everything else — your MINT programs, your debug output, all the normal console traffic — is completely ignored.

It is an elegant solution, and once you have built it you will find yourself thinking of other things you could hang off that wire.

---

## HOW IT WORKS

The TEC-1 talks to your PC terminal over a serial line at 4800 baud. That same wire now feeds two receivers instead of one:

```
TEC-1 TX ──┬──► PC terminal (your console, as normal)
            └──► ATtiny85 pin PB3 (listening)
```

The ATtiny85 firmware watches every byte that arrives. If the first byte is **F** (forward), **R** (reverse), or **S** (stop), the chip knows a motor command is coming and reads the next four bytes to find out which axis and at what speed. Any other byte — and there will be thousands of them from normal MINT activity — is thrown away immediately.

On the MINT side, you load a small set of functions that send motor commands to the console using the normal `/C` output instruction. The ATtiny sees these on the wire and acts on them. Your PC terminal sees them too, but at 4800 baud they flash past in under half a millisecond and cause no visible effect.

The motor commands are five ASCII characters long:

```
<DIR> <AXIS> <NNN>

FX100   ← X axis forward, speed 100
RX050   ← X axis reverse, speed 50
FY255   ← Y axis forward, full speed
SX000   ← Emergency stop
```

Speed runs from 000 to 255 and is passed directly to the ATtiny's PWM outputs. The H-bridge chip turns this into motor voltage.

---

## WHAT YOU NEED

**Microcontroller and programming**

| Qty | Part | Notes |
|-----|------|-------|
| 1 | ATtiny84A (DIP-14) | Main MCU — motors, sensors, serial |
| 1 | 14-pin IC socket | Allows removal for reprogramming |
| 1 | USBasp programmer | Flashes ATtiny via Arduino IDE |
| 6 | 2.54mm pin header | ICSP programming header on board |

**Motor driver**

| Qty | Part | Notes |
|-----|------|-------|
| 1 | L293D (DIP-16) | Preferred over L298N — smaller, no heatsink at low current |
| 1 | 16-pin IC socket | |
| 2 | DC gear motor, 6–12V | One per axis — choose gear ratio to suit your mount |
| 3 | Screw terminal (3-way) | Motor A, Motor B, power — makes wiring detachable |

**Position sensors**

| Qty | Part | Notes |
|-----|------|-------|
| 1 | MPU-6050 breakout (GY-521) | Altitude — I2C addr 0x68 |
| 1 | AS5600 breakout | Azimuth — I2C addr 0x36 |
| 1 | Diametrically magnetised disc magnet | For AS5600 — 6mm dia × 2.5mm typical, mounted on azimuth shaft |

**Passives**

| Qty | Part | Notes |
|-----|------|-------|
| 2 | 4.7kΩ resistor | I2C pull-ups — SDA and SCL to 5V |
| 3 | 100nF ceramic capacitor | Decoupling — ATtiny, L293D logic pin, L293D motor pin |
| 1 | 10µF electrolytic capacitor | Bulk decoupling on motor supply |

**Power**

| Qty | Part | Notes |
|-----|------|-------|
| 1 | 5V regulated supply | Logic — ATtiny84A, sensors, L293D Vss |
| 1 | 6–12V supply | Motors only — L293D Vs, keep separate from logic rail |

**Construction**

| Qty | Part | Notes |
|-----|------|-------|
| 1 | Protoboard | Solder everything — not breadboard |
| — | Hook-up wire | Stranded for motor leads, solid for signal |
| 1 | 3-pin connector | GND + TEC-1 TX (to ATtiny RX) + ATtiny TX (to TEC-1 RX) |

**Software**

- Arduino IDE with ATTinyCore board support
- MINT 2 running on your TEC-1

**Approximate cost:** ~$10–15 AUD in components, excluding motors, TEC-1, and programmer.

---

**Before you finalise the firmware — measure these on your actual hardware:**

| Constant | How to set it |
|----------|---------------|
| `BACKLASH_X_MS` | Command a fast slew, then reverse — time from command to load actually moving |
| `BACKLASH_Y_MS` | Same for Y axis — will differ from X |
| `RAMP_STEP` / `RAMP_MS` | Tune to your motor inertia — watch for vibration or overshoot at stop |
| AS5600 magnet gap | 0.5–3mm — check AS5600 datasheet for your specific magnet dimensions |

---

## THE ATtiny85 PIN MAP

```
ATtiny85 Pin   Arduino#   Function          Connect to
──────────────────────────────────────────────────────
PB0  (pin 5)      0       X forward PWM     H-bridge IN1
PB1  (pin 6)      1       X reverse PWM     H-bridge IN2
PB2  (pin 7)      2       Y reverse PWM     H-bridge IN4
PB3  (pin 2)      3       Serial RX         TEC-1 TX (T-junction)
PB4  (pin 3)      4       Y forward PWM     H-bridge IN3
```

Note that PB3 is the only input. There is no TX back to the TEC-1. Communication is one-way only — the ATtiny takes orders and executes them.

---

## H-BRIDGE WIRING

The L293D is a dual H-bridge in a 16-pin package. Connect it like this:

```
ATtiny PB0 ──► IN1 ─┐
ATtiny PB1 ──► IN2 ─┤  L293D  ├── OUT1/OUT2 ──► X motor
ATtiny PB4 ──► IN3 ─┤         ├── OUT3/OUT4 ──► Y motor
ATtiny PB2 ──► IN4 ─┘

Vss  = 5V  (logic supply, same as ATtiny)
Vs   = 6–12V  (motor supply, keep separate)
GND  = common — tie all grounds together
```

The enable pins (EN1, EN2) can be tied high permanently. Speed control is handled entirely by the PWM duty cycle on the input pins.

**Keep the motor supply separate from the logic supply.** Motors are electrically noisy. Run separate wires from the motor supply to the H-bridge Vs pin, and do not share this rail with the ATtiny or TEC-1.

---

## HOW FORWARD AND REVERSE WORK

There are four PWM output pins — one per direction per axis. Each pair drives one side of the H-bridge:

```
X_FWD (PB0)    X_REV (PB1)
Y_FWD (PB4)    Y_REV (PB2)
```

The firmware always zeroes the opposite pin before applying PWM to the active one. This is critical. If both IN1 and IN2 were driven high at the same moment, the H-bridge would short-circuit the motor supply directly to ground — what is called shoot-through. The firmware prevents this with a simple rule: kill the opposite direction first, then apply the new speed.

| IN1 | IN2 | Motor state |
|-----|-----|-------------|
| PWM | 0   | Forward |
| 0   | PWM | Reverse |
| 0   | 0   | Coast / stop |

Always send a stop command before reversing. The MINT words do this automatically in the slew and nudge functions.

---

## BUILDING THE FIRMWARE

You will need the Arduino IDE with ATTinyCore installed. ATTinyCore adds support for the ATtiny range of chips.

**Installing ATTinyCore:**

1. Open Arduino IDE
2. Go to **File → Preferences** and paste this into the Additional Board Manager URLs box:
   `http://drazzy.com/package_drazzy.com_index.json`
3. Go to **Tools → Board → Board Manager**, search for **ATTinyCore** and install it
4. Select **Tools → Board → ATtiny25/45/85**
5. Set **Chip: ATtiny85** and **Clock: 8 MHz (internal)**
6. Burn the bootloader once to set the fuses correctly
7. Connect your USBasp programmer and upload

**A note on PWM frequency:**

The default ATtiny85 PWM frequency is around 500 Hz. At this frequency you may hear a faint whine from the motors. If this bothers you, add these two lines at the top of `setup()` to push the frequency up to around 32 kHz — well above the range of hearing:

```cpp
TCCR0B = (TCCR0B & 0b11111000) | 0x01;   // Timer0 ~32kHz
TCCR1  = (TCCR1  & 0b10000000) | 0x01;   // Timer1 ~32kHz
```

Start with the default first. Only make this change if the motor hum is actually a problem for you.

---

## FIRMWARE LISTING — xy_telescope.ino

```cpp
/*
 * xy_telescope.ino
 * ATtiny85 telescope X/Y axis DC motor controller
 *
 * Receives serial commands from a TEC-1 SBC (or any TTL serial source)
 * and drives two DC motors via an H-bridge (L293D or L298N).
 *
 * Command format:  <DIR><AXIS><NNN>
 *   DIR  : F = forward, R = reverse, S = stop
 *   AXIS : X or Y
 *   NNN  : speed 000-255 (zero-padded to 3 digits)
 *
 * Examples:
 *   FX100  - X axis forward at speed 100
 *   RX050  - X axis reverse at speed 50
 *   FY255  - Y axis forward, full speed
 *   SX000  - emergency stop all
 *
 * Baud rate: 4800 (keep low for reliable SoftwareSerial on ATtiny85)
 *
 * Pin assignments:
 *   PB0 (Arduino 0) - X forward PWM  -> H-bridge IN1
 *   PB1 (Arduino 1) - X reverse PWM  -> H-bridge IN2
 *   PB2 (Arduino 2) - Y reverse PWM  -> H-bridge IN4
 *   PB3 (Arduino 3) - Serial RX      -> TEC-1 TX (T-junction)
 *   PB4 (Arduino 4) - Y forward PWM  -> H-bridge IN3
 */

#include <SoftwareSerial.h>

#define X_FWD 0
#define X_REV 1
#define Y_REV 2
#define Y_FWD 4
#define RX_PIN 3
#define DUMMY_TX 5

#define BAUD_RATE        4800
#define DIGIT_TIMEOUT_MS 100
#define NUM_DIGITS       3

SoftwareSerial z80Serial(RX_PIN, DUMMY_TX);

void stopAll() {
    analogWrite(X_FWD, 0);
    analogWrite(X_REV, 0);
    analogWrite(Y_FWD, 0);
    analogWrite(Y_REV, 0);
}

void driveMotor(char dir, char axis, uint8_t speed) {
    bool fwd = (dir == 'F');

    if (axis == 'X') {
        analogWrite(fwd ? X_REV : X_FWD, 0);
        analogWrite(fwd ? X_FWD : X_REV, speed);
    } else if (axis == 'Y') {
        analogWrite(fwd ? Y_REV : Y_FWD, 0);
        analogWrite(fwd ? Y_FWD : Y_REV, speed);
    }
}

bool readDigits(char *buf) {
    uint8_t count = 0;
    unsigned long t = millis();

    while (count < NUM_DIGITS) {
        if ((millis() - t) > DIGIT_TIMEOUT_MS) return false;
        if (z80Serial.available()) {
            buf[count++] = z80Serial.read();
        }
    }
    buf[NUM_DIGITS] = '\0';
    return true;
}

void setup() {
    pinMode(X_FWD, OUTPUT);
    pinMode(X_REV, OUTPUT);
    pinMode(Y_FWD, OUTPUT);
    pinMode(Y_REV, OUTPUT);
    stopAll();
    z80Serial.begin(BAUD_RATE);
}

void loop() {
    if (!z80Serial.available()) return;

    char dir = z80Serial.read();

    if (dir == 'S') { stopAll(); return; }
    if (dir != 'F' && dir != 'R') return;

    unsigned long t = millis();
    while (!z80Serial.available()) {
        if ((millis() - t) > DIGIT_TIMEOUT_MS) return;
    }
    char axis = z80Serial.read();
    if (axis != 'X' && axis != 'Y') return;

    char buf[NUM_DIGITS + 1];
    if (!readDigits(buf)) return;

    uint8_t speed = (uint8_t)constrain(atoi(buf), 0, 255);
    driveMotor(dir, axis, speed);
}
```

That is the complete firmware. It is about as simple as a serial motor controller can get. The `loop()` function just waits for a byte, checks whether it is a valid command prefix, then reads the axis and speed and calls `driveMotor()`.

---

## LOADING THE MINT PROGRAM

Once the ATtiny84A is flashed and wired up, you need to load the MINT program into your TEC-1.

**Before you upload**, strip all the comment lines from `xy_telescope.mint`. MINT's input buffer is limited to 256 bytes per line and comment lines waste space. You can strip them manually or use a simple script.

Upload the stripped file to the TEC-1 via picocom or your usual serial terminal at 4800 baud. After each function definition you should see the MINT prompt `>` echoed back, confirming the function was accepted.

This single file contains everything — motor control, position display, goto, tracking, and the star table. There is no second file to load.

**Setting the delay constant:**

The `d` variable controls the timing of the slew and nudge functions. Set it for your board before running any movement commands:

```
50 d!     for a 2MHz TEC-1
100 d!    for a 4MHz TEC-1
```

Verify the timing with a scope or logic analyser: the nudge functions should run the motor for about 50ms. Adjust `d` until this is right.

---

## MINT PROGRAM LISTING — xy_telescope.mint

This single file replaces both the original motor control and position files. Strip comment lines before uploading.

```
100 d!
80 f!
20 s!
100 b!
20 e!
[1060 350 1730 520 2140 500 2250 350 2800 350] o!

:D 100 / /r t! 48 + /C t 10 / /r u! 48 + /C u 48 + /C ;
:M /C /C D ;
:F 88 70 M ;
:R 88 82 M ;
:H 0 F ;
:G 89 70 M ;
:V 89 82 M ;
:J 0 G ;
:S H J ;
:X $ F d * () H ;
:Y $ G d * () J ;
:A 30 50 X ;
:B 30 R 50 d * () H ;
:C 30 50 Y ;
:E 30 V 50 d * () J ;

:N 0 4 ( 10 * /K 48 - + ) ;
:I /U ( /K 65 - /W ) N a! /K ' N z! ;
:P `ALT=` a . `AZ=` z . ;
:K q! p! ;
:L " 2 * o $ ? q! 2 * 1 + o $ ? p! ;
:O p a - " 0 < ( -1 * ) " b > ( ' p a > ( f G ) /E ( f V ) ) /E ( e > ( p a > ( s G ) /E ( s V ) ) /E ( J ) ) ;
:Q a 85 > ( H ) /E ( q z - " 0 < ( -1 * ) " b > ( ' q z > ( f F ) /E ( f R ) ) /E ( e > ( q z > ( s F ) /E ( s R ) ) /E ( H ) ) ) ;
:W /U ( I p a - " 0 < ( -1 * ) e > q z - " 0 < ( -1 * ) e > | /W O Q ) S ;
:T 20 f! 20 s! /U ( I P O Q 500() ) ;
```

**How the MINT code works:**

Function **D** is the key building block. It takes a number from the stack and sends it as three ASCII decimal digits. For example, a speed of 100 becomes the characters `1`, `0`, `0`. A speed of 50 becomes `0`, `5`, `0`.

It does this using MINT's division operator, which leaves the remainder in the system variable `/r`. Divide by 100 to get the hundreds digit, save the remainder, divide by 10 to get the tens digit, and what is left over is the units.

Function **M** sends the complete command. It takes direction, axis, and speed from the stack, emits the direction character, emits the axis character, then calls D for the three-digit speed.

The axis and direction characters are just ASCII values. X is 88, Y is 89, F is 70, R is 82. So **F** (X forward) pushes 88 and 70 onto the stack then calls M. Simple.

The slew functions **X** and **Y** take a speed and a step count. They start the motor, loop for `steps × d` iterations as a time delay, then stop. The `d` variable is your timing calibration constant.

---

## USING THE CONTROLLER

Once everything is loaded and running, controlling the telescope is straightforward. Type at the MINT prompt:

```
100 F        start X axis moving forward at speed 100
H            stop X axis
80 200 Y     slew Y axis forward at speed 80 for ~200ms, then stop
A            brief nudge X forward
B            brief nudge X reverse
S            emergency stop — both axes halt immediately
I P          read current position and display it
325 1573 K W goto altitude 32.5 degrees, azimuth 157.3 degrees (values in tenths)
0 L W        look up star 0 (Sirius) and goto
T            track current target (runs until RESET)
```

**Function reference:**

| Function | Stack effect | What it does |
|----------|-------------|--------------|
| `F` | speed -- | X axis forward |
| `R` | speed -- | X axis reverse |
| `H` | -- | X axis stop |
| `G` | speed -- | Y axis forward |
| `V` | speed -- | Y axis reverse |
| `J` | -- | Y axis stop |
| `S` | -- | Stop all — both axes |
| `X` | speed steps -- | Slew X forward, wait, stop |
| `Y` | speed steps -- | Slew Y forward, wait, stop |
| `A` | -- | Nudge X forward |
| `B` | -- | Nudge X reverse |
| `C` | -- | Nudge Y forward |
| `E` | -- | Nudge Y reverse |
| `I` | -- | Wait for ATtiny84A position update; store in `a`, `z` |
| `P` | -- | Print current position |
| `K` | alt az -- | Set target position |
| `L` | n -- | Load star n from table as target |
| `W` | -- | Goto target, stop on arrival |
| `T` | -- | Track target continuously (runs until RESET) |

Speed is 0 to 255. Zero stops the motor. 255 is full speed. Start with something in the range of 50 to 100 until you know how fast your motors respond.

---

## TESTING

**Test the ATtiny85 first — before connecting the TEC-1.**

Connect a USB-serial adapter (CP2102 or CH340) directly to PB3 at 4800 baud. Open picocom on your PC:

```bash
picocom -b 4800 /dev/ttyUSB0
```

Type `FX100` and press enter. The X motor should start. Type `SX000` to stop it. If you do not have motors handy yet, put LEDs and resistors across the H-bridge outputs — they will light up and show you which direction is active.

Probe PB0, PB1, PB2, and PB4 with a scope or logic analyser. You should see a clean PWM signal on the active pin and zero on the opposite pin. Confirm that shoot-through never occurs — both pins of a pair should never be high at the same time.

**Then test the MINT words on the TEC-1.**

Attach your logic analyser to the TEC-1 TX line. Set the UART decoder to 4800 baud, 8N1.

Type `A` at the MINT prompt. You should see two bursts of five bytes on the analyser:

```
F X 0 3 0    (nudge at speed 30)
F X 0 0 0    (stop)
```

If you see these bytes in the right order, everything is working. Now make the T-junction, connect the ATtiny, and run `A` again. The motor should give a brief twitch forward.

**Minimal test rig:**

| Item | Purpose |
|------|---------|
| ATtiny85 + USBasp | Flash the firmware |
| USB-serial adapter (CP2102 / CH340) | Test ATtiny without the TEC-1 |
| L293D or L298N | H-bridge |
| 2 × DC motors or LEDs + resistors | See the output |
| Logic analyser (LA104 or similar) | Check serial timing and PWM |
| 6–12V motor supply | Separate from logic |

---

## DESIGN NOTES

The ATtiny85 does no thinking. It is a pure coprocessor — all the intelligence stays in the TEC-1 and your MINT programs. The ATtiny just converts serial commands into PWM signals.

Because motor commands use F, R, and S as their first character, and these are perfectly ordinary letters that could appear in MINT output, you might wonder whether stray text could accidentally drive the motors. In practice it is not a problem. A command is only acted on if it is followed by a valid axis character (X or Y) and then three valid digit characters, all arriving within 100ms. The chance of random console output accidentally matching this pattern and arriving in time is vanishingly small.

If you are leaving the telescope unattended, consider adding a watchdog: a MINT loop that sends a stop command if no movement has been requested for some time. If the TEC-1 crashes, the ATtiny will hold the last speed indefinitely.

Have fun with it. The same technique — hanging a small coprocessor off the console serial line — can be applied to many other TEC-1 projects.

---

## ATtiny84A COMBINED FIRMWARE — xy_telescope_84a.ino

This is the full combined firmware for the ATtiny84A. It handles motor control, I2C sensor reading, and angle transmission in one sketch. Load this instead of `xy_telescope.ino` when building the combined board.

```cpp
/*
 * xy_telescope_84a.ino
 * ATtiny84A combined telescope motor controller + position sensor
 *
 * Receives 5-byte motor commands from TEC-1 over serial RX (4800 8N1)
 * Reads MPU-6050 (altitude) and AS5600 (azimuth) over bit-bang I2C
 * Transmits angle string to TEC-1 over serial TX every 500ms when idle
 *
 * Output format: "A<4dig_alt_tenths> <4dig_az_tenths>\n"  e.g. "A0325 1573\n"
 *
 * Pin map (ATTinyCore numbering, ATtiny84A):
 *   PA0  (0)   SDA   bit-bang I2C data
 *   PA1  (1)   SCL   bit-bang I2C clock
 *   PA2  (2)   RX    SoftwareSerial <- TEC-1 TX (motor commands)
 *   PA3  (3)   TX    SoftwareSerial -> TEC-1 RX (angle data)
 *   PA6  (6)   Y_REV PWM OC1B       -> H-bridge IN4
 *   PA7  (7)   X_FWD PWM OC0B       -> H-bridge IN1
 *   PB1  (9)   Y_FWD PWM OC1A       -> H-bridge IN3
 *   PB2  (10)  X_REV PWM OC0A       -> H-bridge IN2
 *
 * I2C: 4k7 pull-ups on SDA and SCL to 5V
 * MPU-6050 addr 0x68, AS5600 addr 0x36 — no conflict, shared bus
 */

#include <SoftwareSerial.h>

#define SDA_PIN  0
#define SCL_PIN  1
#define RX_PIN   2
#define TX_PIN   3

#define X_FWD    7
#define X_REV   10
#define Y_FWD    9
#define Y_REV    6

#define BAUD             4800
#define DIGIT_TIMEOUT_MS  100
#define NUM_DIGITS          3
#define SENSOR_INTERVAL   500
#define RAMP_STEP          20
#define RAMP_MS             8
#define WATCHDOG_MS      5000
#define BACKLASH_X_MS       0
#define BACKLASH_Y_MS       0
#define BACKLASH_SPEED     60

#define MPU6050_ADDR     0x68
#define AS5600_ADDR      0x36

SoftwareSerial tec1(RX_PIN, TX_PIN);

// ── Bit-bang I2C ─────────────────────────────────────────────────────────────

void sda_low()  { pinMode(SDA_PIN, OUTPUT); }
void sda_high() { pinMode(SDA_PIN, INPUT);  }
void scl_low()  { pinMode(SCL_PIN, OUTPUT); }
void scl_high() { pinMode(SCL_PIN, INPUT);  }

void i2c_init() {
    digitalWrite(SDA_PIN, LOW); digitalWrite(SCL_PIN, LOW);
    sda_high(); scl_high();
}

void i2c_start() {
    sda_high(); scl_high(); delayMicroseconds(4);
    sda_low();              delayMicroseconds(4);
    scl_low();
}

void i2c_stop() {
    sda_low();
    scl_high(); delayMicroseconds(4);
    sda_high(); delayMicroseconds(4);
}

bool i2c_write_byte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
        if (b & 0x80) sda_high(); else sda_low();
        b <<= 1;
        scl_high(); delayMicroseconds(4);
        scl_low();  delayMicroseconds(2);
    }
    sda_high();
    scl_high(); delayMicroseconds(4);
    bool ack = (digitalRead(SDA_PIN) == LOW);
    scl_low();
    return ack;
}

uint8_t i2c_read_byte(bool send_ack) {
    uint8_t b = 0;
    sda_high();
    for (uint8_t i = 0; i < 8; i++) {
        scl_high(); delayMicroseconds(4);
        b = (b << 1) | digitalRead(SDA_PIN);
        scl_low();  delayMicroseconds(2);
    }
    if (send_ack) sda_low(); else sda_high();
    scl_high(); delayMicroseconds(4);
    scl_low(); sda_high();
    return b;
}

bool i2c_read_regs(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
    i2c_start();
    if (!i2c_write_byte(addr << 1)) { i2c_stop(); return false; }
    i2c_write_byte(reg);
    i2c_start();
    i2c_write_byte((addr << 1) | 1);
    for (uint8_t i = 0; i < len; i++)
        buf[i] = i2c_read_byte(i < len - 1);
    i2c_stop();
    return true;
}

bool i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t val) {
    i2c_start();
    if (!i2c_write_byte(addr << 1)) { i2c_stop(); return false; }
    i2c_write_byte(reg); i2c_write_byte(val);
    i2c_stop();
    return true;
}

// ── Motor control ─────────────────────────────────────────────────────────────

static uint8_t x_speed = 0, y_speed = 0;
static bool    x_fwd   = true, y_fwd = true;

void stopAll() {
    x_speed = 0; y_speed = 0;
    analogWrite(X_FWD, 0); analogWrite(X_REV, 0);
    analogWrite(Y_FWD, 0); analogWrite(Y_REV, 0);
}

void driveMotor(char dir, char axis, uint8_t speed) {
    bool fwd = (dir == 'F');
    if (axis == 'X') {
        if (speed == 0 && x_speed > RAMP_STEP) {
            while (x_speed > 0) {
                x_speed = (x_speed > RAMP_STEP) ? x_speed - RAMP_STEP : 0;
                analogWrite(x_fwd ? X_FWD : X_REV, x_speed);
                delay(RAMP_MS);
            }
        }
        if (speed > 0 && x_speed > 0 && fwd != x_fwd && BACKLASH_X_MS > 0) {
            analogWrite(x_fwd ? X_FWD : X_REV, 0);
            analogWrite(fwd  ? X_FWD : X_REV, BACKLASH_SPEED);
            delay(BACKLASH_X_MS);
        }
        x_fwd = fwd; x_speed = speed;
        analogWrite(fwd ? X_REV : X_FWD, 0);
        analogWrite(fwd ? X_FWD : X_REV, speed);
    } else if (axis == 'Y') {
        if (speed == 0 && y_speed > RAMP_STEP) {
            while (y_speed > 0) {
                y_speed = (y_speed > RAMP_STEP) ? y_speed - RAMP_STEP : 0;
                analogWrite(y_fwd ? Y_FWD : Y_REV, y_speed);
                delay(RAMP_MS);
            }
        }
        if (speed > 0 && y_speed > 0 && fwd != y_fwd && BACKLASH_Y_MS > 0) {
            analogWrite(y_fwd ? Y_FWD : Y_REV, 0);
            analogWrite(fwd  ? Y_FWD : Y_REV, BACKLASH_SPEED);
            delay(BACKLASH_Y_MS);
        }
        y_fwd = fwd; y_speed = speed;
        analogWrite(fwd ? Y_REV : Y_FWD, 0);
        analogWrite(fwd ? Y_FWD : Y_REV, speed);
    }
}

bool readDigits(char *buf) {
    uint8_t count = 0;
    unsigned long t = millis();
    while (count < NUM_DIGITS) {
        if ((millis() - t) > DIGIT_TIMEOUT_MS) return false;
        if (tec1.available()) buf[count++] = tec1.read();
    }
    buf[NUM_DIGITS] = '\0';
    return true;
}

static unsigned long lastSerialMs = 0;

void handleSerial() {
    if (!tec1.available()) return;
    lastSerialMs = millis();
    char dir = tec1.read();
    if (dir == 'S') { stopAll(); return; }
    if (dir != 'F' && dir != 'R') return;
    unsigned long t = millis();
    while (!tec1.available()) {
        if ((millis() - t) > DIGIT_TIMEOUT_MS) return;
    }
    char axis = tec1.read();
    if (axis != 'X' && axis != 'Y') return;
    char buf[NUM_DIGITS + 1];
    if (!readDigits(buf)) return;
    driveMotor(dir, axis, (uint8_t)constrain(atoi(buf), 0, 255));
}

// ── Arctangent lookup table ───────────────────────────────────────────────────
// tan_lut[i] = round(tan(i degrees) * 1000), i = 0..89

static const uint16_t tan_lut[90] PROGMEM = {
      0,  17,  35,  52,  70,  87, 105, 123, 141, 158,
    176, 194, 213, 231, 249, 268, 287, 306, 325, 344,
    364, 384, 404, 424, 445, 466, 488, 510, 532, 554,
    577, 601, 625, 649, 674, 700, 727, 754, 781, 810,
    839, 869, 900, 933, 966,1000,1036,1072,1111,1150,
   1192,1235,1280,1327,1376,1428,1483,1540,1600,1664,
   1732,1804,1881,1963,2050,2145,2246,2356,2475,2605,
   2747,2904,3078,3271,3487,3732,4011,4331,4705,5145
};

uint8_t atan_deg(uint16_t ratio) {
    for (uint8_t i = 0; i < 89; i++) {
        if (ratio < pgm_read_word(&tan_lut[i + 1])) return i;
    }
    return 89;
}

// ── MPU-6050 ──────────────────────────────────────────────────────────────────

void mpu6050_init() {
    i2c_write_reg(MPU6050_ADDR, 0x6B, 0x00);
    delay(10);
}

// Returns altitude in centidegrees (e.g. 3214 = 32.14 degrees), -1 on error
int16_t read_altitude_centideg() {
    uint8_t buf[6];
    if (!i2c_read_regs(MPU6050_ADDR, 0x3B, buf, 6)) return -1;
    int16_t ay = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t az = (int16_t)((buf[4] << 8) | buf[5]);
    uint16_t num = (uint16_t)abs(ay);
    uint16_t den = (uint16_t)abs(az);
    if (den == 0) return 9000;
    uint16_t ratio = (uint16_t)((uint32_t)num * 1000 / den);
    uint8_t deg = atan_deg(ratio);
    uint16_t lo = pgm_read_word(&tan_lut[deg]);
    uint16_t hi = (deg < 89) ? pgm_read_word(&tan_lut[deg + 1]) : 10000;
    uint8_t frac = (hi == lo) ? 0 : (uint8_t)((uint32_t)(ratio - lo) * 100 / (hi - lo));
    return (int16_t)deg * 100 + frac;
}

// ── AS5600 ────────────────────────────────────────────────────────────────────

// Returns azimuth in centidegrees (e.g. 15708 = 157.08 degrees), -1 on error
int32_t read_azimuth_centideg() {
    uint8_t buf[2];
    if (!i2c_read_regs(AS5600_ADDR, 0x0E, buf, 2)) return -1;
    uint16_t raw = ((uint16_t)(buf[0] & 0x0F) << 8) | buf[1];
    return (int32_t)raw * 36000L / 4096L;
}

// ── Angle output ──────────────────────────────────────────────────────────────
// Sends "A<4dig_alt_tenths> <4dig_az_tenths>\n"  e.g. "A0325 1573\n"
// Tenths of a degree: alt 0-900, az 0-3599 — fits MINT 16-bit signed (max 32767)

void transmit_angles(int16_t alt_cd, int32_t az_cd) {
    uint16_t alt_td = (uint16_t)(alt_cd / 10);
    uint16_t az_td  = (uint16_t)(az_cd  / 10);
    tec1.print('A');
    tec1.print(alt_td / 1000); tec1.print((alt_td / 100) % 10); tec1.print((alt_td / 10) % 10); tec1.print(alt_td % 10);
    tec1.print(' ');
    tec1.print(az_td / 1000);  tec1.print((az_td / 100) % 10);  tec1.print((az_td / 10) % 10);  tec1.print(az_td % 10);
    tec1.print('\n');
}

// ── Setup & loop ──────────────────────────────────────────────────────────────

void setup() {
    pinMode(X_FWD, OUTPUT); pinMode(X_REV, OUTPUT);
    pinMode(Y_FWD, OUTPUT); pinMode(Y_REV, OUTPUT);
    stopAll();
    i2c_init();
    tec1.begin(BAUD);
    mpu6050_init();
}

void loop() {
    static unsigned long lastSensor = 0;
    handleSerial();
    if ((x_speed > 0 || y_speed > 0) && (millis() - lastSerialMs) > WATCHDOG_MS)
        stopAll();
    if ((millis() - lastSensor) >= SENSOR_INTERVAL && !tec1.available()) {
        int16_t alt = read_altitude_centideg();
        int32_t az  = read_azimuth_centideg();
        if (alt >= 0 && az >= 0) transmit_angles(alt, az);
        lastSensor = millis();
    }
}
```

**How the sensor code works:**

The I2C bus is bit-banged on PA0 and PA1. Open-drain style — the pin is either driven LOW or released to float HIGH via the 4k7 pull-up resistors. Both sensors share the same two wires.

The MPU-6050 is woken up on startup (the chip defaults to sleep mode). Every 500ms the firmware reads six bytes of raw accelerometer data — two bytes each for X, Y, and Z axes. The altitude angle is the arctangent of the ratio of two of these axes, found by searching the 90-entry lookup table stored in flash. Interpolation between table entries gives sub-degree resolution in arc-minutes and arc-seconds without needing floating point.

The AS5600 delivers its 12-bit angle register in two bytes. Multiply by 36000 and divide by 4096 to get centidegrees. The maths stays in 32-bit integers throughout.

Sensor reads only happen when the serial line is idle. This avoids any conflict between the bit-bang I2C timing and the SoftwareSerial interrupt handler waiting for incoming motor commands.

**Mounting note:**

The altitude calculation uses the MPU-6050 Y and Z axes. Adjust the `ay`/`az` axis selection in `read_altitude_centideg()` to match however the chip is physically mounted on your telescope.

---

## WHERE TO GO FROM HERE — POSITION SENSING

Right now this system is open-loop. You tell the motors to run for a certain time at a certain speed, and you trust that the telescope moved roughly where you wanted it. For casual use that is fine. But if you want to point at a specific star — or track one as the Earth rotates — you need to know the actual angles.

The good news is that adding position sensing is a natural next step, and the hardware is cheap.

**For the altitude axis (Y), use an MPU-6050.**

This is a small I2C chip that contains a 3-axis accelerometer and a 3-axis gyroscope. The accelerometer measures tilt relative to gravity, which gives you absolute altitude angle directly — no reference point, no homing, no counting pulses. It just tells you how far the telescope is tilted from horizontal. Accuracy is better than half a degree, which is more than adequate for visual astronomy.

**For the azimuth axis (X), use an AS5600.**

This is a magnetic absolute encoder. You glue a small diametrically magnetised disc magnet to the rotating shaft and position the AS5600 chip underneath it. As the shaft rotates, the chip measures the angle of the magnetic field and reports a 12-bit value over I2C. That gives you 4096 steps over a full rotation — about 0.09 degrees per step.

Do not be tempted to use a compass module for azimuth. The motors are too close and their magnetic fields will corrupt the reading completely.

**Can one chip do everything?**

It is a fair question. The motor controller ATtiny85 has all five of its pins committed, so it cannot take on sensor reading as well. But rather than add a second ATtiny85, there is a better option — the **ATtiny84A**.

This is the natural step up from the ATtiny85. It is the same family, uses the same Arduino IDE setup and ATTinyCore board support, comes in a DIP-14 package that sits comfortably on protoboard, and costs about the same — around one to two dollars. The important difference is that it has 12 I/O pins instead of five. That is enough to handle motor driving, I2C sensor reading, and serial communication all at once, with four pins to spare.

Here is how the pins are assigned:

```
ATtiny84A — combined motor and sensor controller
──────────────────────────────────────────────────────
PA0   X_FWD   PWM (OC0A)   → H-bridge IN1
PA1   X_REV   PWM           → H-bridge IN2
PA2   Y_FWD   PWM           → H-bridge IN3
PA3   Y_REV   PWM (OC1B)   → H-bridge IN4
PA4   SCL     I2C clock     → MPU-6050 + AS5600
PA6   SDA     I2C data      → MPU-6050 + AS5600
PB0   RX      serial in   ← TEC-1 TX (motor commands)
PB1   TX      serial out  → TEC-1 RX (angle data)
```

The MPU-6050 and AS5600 have different I2C addresses — 0x68 and 0x36 — so they share the same two wires without any conflict or multiplexer.

The complete system wiring becomes:

```
TEC-1 TX ──┬──► PC terminal            (console, unchanged)
            └──► ATtiny84A PB0 RX      (motor commands in)

ATtiny84A PB1 TX ──────────────────► TEC-1 RX   (angle data out)

ATtiny84A PA4/PA6 I2C ──┬──► MPU-6050  (altitude)
                         └──► AS5600    (azimuth)

ATtiny84A PA0–PA3 PWM ──────────────► H-bridge → motors
```

One chip. One protoboard. Everything in one place.

**Is the processing load too much?**

No — and the reason is straightforward. At 4800 baud a motor command arrives roughly every two milliseconds and takes about one millisecond to receive. Between commands the chip is sitting idle. An I2C sensor read at 400kHz takes under half a millisecond. The PWM outputs run entirely in hardware once set up — they consume zero CPU time. The chip simply reads the sensors during the quiet gaps between commands and never touches I2C while a command is arriving. At 8MHz the ATtiny84A has far more headroom than this task demands.

**How the conversion works:**

The AS5600 delivers a 12-bit number from 0 to 4095 representing one full rotation. Multiply by 360 and divide by 4096 to get decimal degrees, then split into degrees, minutes, and seconds using integer arithmetic. No floating point anywhere.

The MPU-6050 delivers raw accelerometer values on three axes. The altitude angle is the arctangent of the ratio of two of these values. Rather than computing arctangent in software — which needs floating point and is slow — a 90-entry lookup table stored in flash gives one value per degree. Interpolate between entries for the minutes and seconds. The table costs 180 bytes of the ATtiny84A's 8k flash.

The chip transmits the result as a short serial string at 4800 baud:

```
A0325 1573
```

That is the letter `A`, then altitude in tenths of degrees (four digits, zero-padded), a space, and azimuth in tenths of degrees. `0325` means 32.5°, `1573` means 157.3°. Tenths fit MINT's 16-bit signed integers for both axes (max alt 900, max az 3599, both well under 32767) and use the full resolution of the AS5600 encoder.

Use **bit-bang I2C** on PA0 and PA1 rather than the ATtiny84A's USI peripheral. The USI shares pins with two of the PWM outputs, which would mean losing motor channels. Bit-bang is simple and reliable at the speeds needed here.

---

## MINT POSITION CONTROL

The position control functions are built into `xy_telescope.mint` — there is nothing extra to load. Once the ATtiny84A is sending angle data back to the TEC-1, these functions are ready to use immediately.

**How it works:**

The star table `o` holds five stars as pairs of azimuth and altitude in tenths of degrees. The layout is `[az alt az alt ...]` so star 0 occupies indices 0 and 1, star 1 occupies indices 2 and 3, and so on. `1060` = 106.0°, `350` = 35.0°. The coordinates shown are approximate for Melbourne, Australia in January at 9pm. Update them for your location and observation time — multiply each degree value by 10.

Function **N** reads three ASCII digit characters from the serial port and builds them into a number. `0 3 ( 10 * /K 48 - + )` loops three times: each pass multiplies the accumulator by 10, reads a character, subtracts 48 to get the digit value, and adds it in.

Function **I** waits for the `A` prefix byte from the ATtiny84A by looping until it receives character 65. The loop exits when `65 -` produces zero, which is the false value that `/W` breaks on. It then calls `N` twice to read altitude and azimuth into variables `a` and `z`.

Functions **O** and **Q** each handle one axis. They compute the absolute error between target and current, compare it against the threshold `e`, and either drive the motor toward the target or stop it. The absolute value is computed with `" 0 < ( -1 * )` — duplicate the value, test if negative, negate if so.

Function **W** is the goto loop. On each pass it reads a fresh position with `I`, computes whether either axis is still outside the threshold, and uses bitwise OR and `/W` to continue looping as long as any axis needs correction. When both axes are within `e` degrees, the OR produces zero, `/W` breaks, and `S` stops all motors.

Function **T** is the tracking loop. It sets a slow motor speed (20) and runs forever: read position, display it, apply one correction step to each axis, pause 500 loop iterations, repeat. It will keep the telescope centred on the target as the Earth rotates. Press RESET to stop.

**Using it:**

```
I P               read and display current position
325 1573 K        set target: altitude 32.5 degrees, azimuth 157.3 degrees (tenths)
W                 goto target
0 L W             look up star 0 (Sirius) and goto
T                 track (runs until RESET)
```

**Star table index:**

| Index | Star | Az | Alt |
|-------|------|----|-----|
| 0 | Sirius | 106° | 35° |
| 1 | Canopus | 173° | 52° |
| 2 | Achernar | 214° | 50° |
| 3 | Alpha Centauri | 225° | 35° |
| 4 | Rigel | 280° | 35° |

These are approximate values for Melbourne (~37°S) in January at 9pm AEST. Replace them with values from a planetarium program for your location and date before use.

**What you can do once you have the angles:**

Once the TEC-1 knows where the telescope is pointing, MINT can do some interesting things. It can display the current azimuth and altitude as numbers. It can accept a target position and drive both motors until the angles match — a simple goto. It can run a slow continuous correction loop to counteract the Earth's rotation and keep a star centred — basic tracking. And with a small table of star positions stored in MINT variables, it can look up objects by name.

The goto loop in MINT is straightforward in principle. Read the current angle, compare it to the target, drive the motor toward the target at a speed proportional to the error, and stop when you are close enough. The functions you already have — F, R, H, G, V, J — are exactly what that loop needs.

Start with the MPU-6050 on the altitude axis. It is one chip, two wires, and absolute angle with no moving parts. Get that reading correctly in MINT, then add the AS5600 for azimuth. That gives you a fully position-aware telescope controller built around a TEC-1 — which is a remarkable thing.

---

## SOFTWARE YOU WILL NEED

```bash
sudo apt install arduino          # Arduino IDE
sudo apt install picocom          # serial terminal
sudo apt install sigrok pulseview # logic analyser software
```

ATTinyCore must be installed through the Arduino IDE board manager — see the Building the Firmware section above.

---

## WHAT WORKS, WHAT DOESN'T, AND WHAT'S NEXT

This is a good moment to step back and take an honest look at what has been built.

### WHAT WORKS WELL

**The passive sniffer** is the cleverest part of the design. Putting a T-junction on the console TX line and letting the ATtiny eavesdrop costs nothing on the Z80 side. No extra chip, no extra pin, no changes to MINT or the TEC-1 firmware. The ATtiny just watches the traffic and acts on the commands that are meant for it. That is elegant engineering.

**Putting the high-level logic in MINT** is the right call. Star tables, tracking loops, and goto logic can all be edited at the MINT prompt without touching a soldering iron or a USB programmer. If the star coordinates are wrong, you fix them in thirty seconds. If you want a larger threshold or a slower tracking speed, you change one variable. This is exactly what an interpreted language is for.

**Combining motors and sensors in the ATtiny84A** keeps the hardware simple. One chip handles everything between the TEC-1 and the telescope. The 84A has enough pins for bit-bang I2C, two serial lines, and four PWM outputs simultaneously — tight, but manageable.

**32kHz PWM** deserves a mention. Shifting the timer frequency above the audible range eliminates the whine that would otherwise follow your telescope around the garden. It is a small change with a noticeable effect on the usability of the instrument.

### ISSUES — IN ORDER OF IMPORTANCE

Here are the known issues and improvements, ranked by how much they matter in practice.

**1. Watchdog** — implemented. The firmware tracks `lastSerialMs`, reset every time a byte arrives. If any motor is running and five seconds pass without serial traffic, `stopAll()` fires automatically. Set `WATCHDOG_MS` in the firmware constants to adjust the timeout.

**2. Backlash compensation** — implemented, disabled by default. `driveMotor()` detects direction changes on running axes and fires a short pulse in the new direction before applying the commanded speed. Set `BACKLASH_X_MS` and `BACKLASH_Y_MS` in the firmware constants. Both are zero until you measure your gearbox: command a fast slew, then reverse — time from the reversal command to when the load actually moves. That is your backlash duration. It will be different for X and Y. `BACKLASH_SPEED` (default 60) is the PWM during the pulse.

**3. Zenith singularity** — implemented. `Q` now starts with `a 85 > ( H ) /E (...)`. Above 85° altitude, the azimuth motor is stopped and the function returns immediately. Below 85°, the two-speed correction runs.

**4. Bang-bang goto overshoot** — implemented. `O` and `Q` now use two speeds. Beyond `b` degrees from the target (default 10°), the motor runs at `f` (fast, 80). Within `b` degrees but outside `e` (2°), it drops to `s` (slow, 20). Below `e` it stops. Tracking (`T`) sets both `f` and `s` to 20 so it never enters the fast zone. Tune `b`, `f`, and `s` at the MINT prompt for your mount and observation type.

**5. Acceleration ramping** — implemented. A full-speed slew to an instant stop puts mechanical shock through the gears. The firmware now ramps down when a stop command arrives and the current speed is above 20 counts. Each step reduces PWM by 20 and waits 8ms, giving roughly 100 milliseconds total at full speed. Normal stops from the MINT `H` and `J` functions use the ramp. Emergency stop (`S`) bypasses it and halts immediately. Adjust `RAMP_STEP` and `RAMP_MS` at the top of the firmware if your gearbox needs a longer or shorter ramp.

**6. Serial buffer overrun** — unlikely in practice but worth knowing. If the TEC-1 sends a long burst of console text at 4800 baud at exactly the wrong moment, the SoftwareSerial buffer could overflow and a motor command could be lost. Monitor for this during testing; fix only if you actually see missed commands.

**7. Fixed-point precision** — implemented. The serial format now sends tenths of degrees (four-digit zero-padded). MINT variables, thresholds, and the star table all use tenths. The AS5600 encoder is rated to 0.1° so this uses its full resolution. The MPU-6050 is hardware-limited to about ±0.5° regardless — replacing it with a second AS5600 on the altitude axis is the path to astrophotography-grade pointing.

**8. Sidereal clock** — the star table coordinates are only accurate for the date and time they were entered. For regular use across multiple nights, a DS3231 real-time clock on the I2C bus would let the TEC-1 calculate the exact current position of any object from first principles. That is the upgrade that turns this from a demonstration into a genuine observing instrument.

### THE BIGGER PICTURE

You have built a working GOTO telescope controller around a Z80 processor that dates from 1976 and an eight-pin microcontroller. Commercial GOTO controllers cost hundreds of dollars and run embedded operating systems on 32-bit ARM cores. This design does the same fundamental job with a fraction of the resources.

The constraint is not a weakness. It is the point. Every byte matters, every pin is shared, every lookup table is hand-crafted. The result is a system you understand completely, can debug with a logic analyser and a serial terminal, and can modify without a toolchain more complex than the Arduino IDE and the MINT prompt.

Items 1 through 7 are all implemented. What remains is the sidereal RTC (item 8) and serial buffer monitoring (item 6, monitor only). Build it, use it, then revisit those if they matter to you.
