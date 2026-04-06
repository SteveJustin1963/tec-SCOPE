# xy-attiny85 — Telescope X/Y Motor Controller

ATtiny84A acting as a PWM coprocessor and position sensor hub for a TEC-1 Z80 SBC running MINT 2.
Controls two DC motors (telescope X/Y axes) via an H-bridge, and reads an MPU-6050 (altitude) and AS5600 (azimuth) to report position back to the TEC-1.

The ATtiny84A **passively sniffs the TEC-1's console serial TX line** — no
dedicated serial port or driver is needed on the Z80 side. Motor commands
are embedded in the normal MINT serial stream; the ATtiny picks them out
and ignores everything else. Position updates are sent back over a second serial wire.

---

## Files

| File | Purpose |
|------|---------|
| `xy_telescope.ino` | ATtiny85 firmware — motor control only (reference) |
| `xy_telescope_84a.ino` | ATtiny84A firmware — motor control + MPU-6050 + AS5600 |
| `xy_telescope.mint` | MINT 2 — motor control + position display, goto, tracking, star table |

---

## Hardware

### ATtiny85 pin map

```
ATtiny85 Pin  Arduino#  Function          Connect to
─────────────────────────────────────────────────────
PB0  Pin 5    0         X forward PWM     H-bridge IN1
PB1  Pin 6    1         X reverse PWM     H-bridge IN2
PB2  Pin 7    2         Y reverse PWM     H-bridge IN4
PB3  Pin 2    3         Serial RX (TTL)   TEC-1 TX
PB4  Pin 3    4         Y forward PWM     H-bridge IN3
```

### H-bridge (L293D or L298N)

```
ATtiny PB0 ──► IN1 ─┐
ATtiny PB1 ──► IN2 ─┤  L293D  ├── OUT1/OUT2 ──► X motor
ATtiny PB4 ──► IN3 ─┤         ├── OUT3/OUT4 ──► Y motor
ATtiny PB2 ──► IN4 ─┘

Vss  = 5V (logic, shared with ATtiny)
Vs   = motor supply (6–12V, separate)
GND  = all grounds tied together (TEC-1 + ATtiny + motor supply)
```

### Serial wiring

- **4800 baud, 8N1, TTL 5V only**
- The ATtiny85 taps the TEC-1's console TX line with a T-junction:

```
TEC-1 TX ──┬──► PC terminal (console RX)
            └──► ATtiny85 PB3 (RX)
```

- No dedicated serial port is needed on the Z80. The ATtiny eavesdrops on
  the existing console stream and silently discards anything that isn't a
  motor command (`F`/`R`/`S` prefix). Normal MINT console output is
  unaffected.
- The ATtiny PB3 input is high-impedance (CMOS, ~1–10µA leakage) and does
  not load the TX line. Driving two RX inputs from one TX output is fine —
  both loads are negligible and at 4800 baud the 208µs bit period is far
  too slow for capacitive loading to matter. No buffer or resistor needed.
- No TX back to TEC-1 (one-way).
- If the TEC-1 uses RS-232 levels (±12V), add a MAX232 before the T-junction.

---

## Parts list

### Microcontroller and programming

| Qty | Part | Notes |
|-----|------|-------|
| 1 | ATtiny84A (DIP-14) | Main MCU — motors, sensors, serial |
| 1 | 14-pin IC socket | Allows removal for reprogramming |
| 1 | USBasp programmer | Flashes ATtiny via Arduino IDE |
| 6 | 2.54mm pin header | ICSP programming header on board |

### Motor driver

| Qty | Part | Notes |
|-----|------|-------|
| 1 | L293D (DIP-16) | Preferred over L298N — smaller, no heatsink at low current |
| 1 | 16-pin IC socket | |
| 2 | DC gear motor, 6–12V | One per axis — choose gear ratio to suit your mount |
| 3 | Screw terminal (3-way) | Motor A, Motor B, power — makes wiring detachable |

### Position sensors

| Qty | Part | Notes |
|-----|------|-------|
| 1 | MPU-6050 breakout (GY-521) | Altitude — I2C addr 0x68 |
| 1 | AS5600 breakout | Azimuth — I2C addr 0x36 |
| 1 | Diametrically magnetised disc magnet | For AS5600 — 6mm dia × 2.5mm typical, mounted on azimuth shaft |

### Passives

| Qty | Part | Notes |
|-----|------|-------|
| 2 | 4.7kΩ resistor | I2C pull-ups — SDA and SCL to 5V |
| 3 | 100nF ceramic capacitor | Decoupling — ATtiny, L293D logic pin, L293D motor pin |
| 1 | 10µF electrolytic capacitor | Bulk decoupling on motor supply |

### Power

| Qty | Part | Notes |
|-----|------|-------|
| 1 | 5V regulated supply | Logic — ATtiny84A, sensors, L293D Vss |
| 1 | 6–12V supply | Motors only — L293D Vs, kept separate from logic rail |

### Construction

| Qty | Part | Notes |
|-----|------|-------|
| 1 | Protoboard | Solder everything — not breadboard |
| — | Hook-up wire | Stranded for motor leads, solid for signal |
| 1 | 3-pin connector | GND + TEC-1 TX (to ATtiny RX) + ATtiny TX (to TEC-1 RX) |

### Things to measure before setting firmware constants

| Constant | How to measure |
|----------|----------------|
| `BACKLASH_X_MS` | Command fast slew, then reverse — time from command to load moving |
| `BACKLASH_Y_MS` | Same for Y axis — will differ from X |
| `RAMP_STEP` / `RAMP_MS` | Tune to your motor inertia — check for vibration at stop |
| AS5600 magnet gap | 0.5–3mm — check AS5600 datasheet for your magnet size |

**Approximate component cost:** ~$10–15 AUD excluding motors, TEC-1, and programmer.

---

## Software to install

**Arduino IDE + ATTinyCore**

```bash
sudo apt install arduino
```

ATTinyCore must be installed via the GUI — the 1.8.x CLI cannot install board packages reliably:

1. Open Arduino IDE
2. `File → Preferences` → paste into **Additional Board Manager URLs**:
   ```
   http://drazzy.com/package_drazzy.com_index.json
   ```
3. `Tools → Board → Board Manager` → search **ATTinyCore** → Install
4. `Tools → Board → ATtiny25/45/85`, set **Chip: ATtiny85**, **Clock: 8 MHz (internal)**
5. `Sketch → Verify/Compile` to confirm the firmware compiles for the target chip

**Serial terminal**

```bash
sudo apt install picocom
sudo usermod -aG dialout $USER   # re-login after
```

Usage: `picocom -b 4800 /dev/ttyUSB0`

**Logic analyser software**

```bash
sudo apt install sigrok pulseview
```

Add a UART decoder at 4800 baud 8N1 in PulseView to decode the serial stream. Works with cheap LA104 / Saleae clones.

| Tool | Package | Purpose |
|------|---------|---------|
| Arduino IDE | `arduino` | Flash ATtiny85 firmware |
| ATTinyCore | Arduino board manager | ATtiny85 board support |
| picocom | `picocom` | Send test commands over serial |
| PulseView | `sigrok pulseview` | Decode serial + PWM on logic analyser |

---

## Building the firmware

1. Install [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore) in Arduino IDE
2. Board: `ATtiny25/45/85`
3. Chip: `ATtiny85`
4. Clock: `8 MHz (internal)` — burn bootloader to set fuses first
5. Programmer: USBasp or similar
6. Open `xy_telescope.ino` and upload

### PWM frequency note

Default ATtiny85 PWM is ~500 Hz. This can cause audible motor hum.
To raise it, add to `setup()`:

```cpp
// Timer0: ~32kHz
TCCR0B = (TCCR0B & 0b11111000) | 0x01;
// Timer1: ~32kHz
TCCR1 = (TCCR1 & 0b10000000) | 0x01;
```

Test at default first; only change if hum is a problem.

---

## MINT usage

Strip comment lines from `xy_telescope.mint` then upload to the TEC-1 MINT
interpreter via picocom or your serial terminal. The single file includes motor
control, position display, goto, and tracking.

```
100 F          X forward at speed 100 (continuous)
H              halt X
80 200 Y       Y forward at speed 80 for ~200ms, then stop
A              nudge X forward
S              emergency stop both axes
I P            read current position and display it
325 1573 K W   goto altitude 32.5°, azimuth 157.3° (values in tenths)
0 L W          look up star 0 (Sirius) and goto
T              track current target (runs until RESET)
```

### Function reference

| Function | Stack | Action |
|----------|-------|--------|
| `F` | speed -- | X forward |
| `R` | speed -- | X reverse |
| `H` | -- | X stop |
| `G` | speed -- | Y forward |
| `V` | speed -- | Y reverse |
| `J` | -- | Y stop |
| `S` | -- | stop all |
| `X` | speed steps -- | slew X fwd, wait steps*d iters, stop |
| `Y` | speed steps -- | slew Y fwd, wait steps*d iters, stop |
| `A` | -- | nudge X forward (speed 30, 50*d iters) |
| `B` | -- | nudge X reverse |
| `C` | -- | nudge Y forward |
| `E` | -- | nudge Y reverse |
| `I` | -- | block until ATtiny84A sends position; store in `a` (alt), `z` (az) |
| `P` | -- | print current position to console |
| `K` | alt_tenths az_tenths -- | set target position (e.g. `325 1573 K` = 32.5°, 157.3°) |
| `L` | n -- | load star n from table as target |
| `W` | -- | goto target, stop on arrival |
| `T` | -- | track target continuously (runs until RESET) |

### Delay tuning

The `d` variable sets the loop iteration count per millisecond. Adjust it
so that `1 d * ()` takes approximately 1ms on your board:

```
50 d!     2MHz board
100 d!    4MHz board
```

Verify with a scope or logic analyser: `A` should run the motor for ~50ms.

---

## Command format

The ATtiny85 expects a 5-byte ASCII string:

```
<DIR><AXIS><NNN>
```

| Field | Values | Meaning |
|-------|--------|---------|
| DIR | `F` | Forward |
| DIR | `R` | Reverse |
| DIR | `S` | Emergency stop (any axis) |
| AXIS | `X` | X / Azimuth axis |
| AXIS | `Y` | Y / Altitude axis |
| NNN | `000`–`255` | Speed (zero-padded to 3 digits) |

Examples:

```
FX100   X axis forward, speed 100
RX050   X axis reverse, speed 50
FY255   Y axis forward, full speed
RY000   Y axis stop
SX000   Emergency stop all
```

The motor functions in `xy_telescope.mint` build and emit these strings automatically.

---

## How forward and reverse work

There are four PWM output pins — one per direction per axis:

```
X_FWD (PB0)  X_REV (PB1)
Y_FWD (PB4)  Y_REV (PB2)
```

Each pair connects to IN1/IN2 (or IN3/IN4) of the H-bridge. In `driveMotor()`
the opposite pin is always zeroed before the active pin is set:

```cpp
analogWrite(fwd ? X_REV : X_FWD, 0);     // kill opposite direction
analogWrite(fwd ? X_FWD : X_REV, speed); // apply PWM to active direction
```

If both IN1 and IN2 were high simultaneously the H-bridge would short the
motor supply (shoot-through). Only one side is ever active at a time.

| IN1 | IN2 | Motor |
|-----|-----|-------|
| PWM | 0   | forward |
| 0   | PWM | reverse |
| 0   | 0   | coast/stop |

---

## Testing

### Step 1 — ATtiny85 alone

- Flash firmware via USBasp
- Connect a USB-serial adapter (CP2102 / CH340) at 4800 baud directly to PB3 — bypasses the TEC-1 entirely
- Send commands from picocom: `FX100`, `SX000`, etc.
- Probe PB0–PB4 with a logic analyser or scope to confirm PWM output and that shoot-through never occurs
- Replace motors with LEDs + resistors on the H-bridge outputs to verify direction logic safely

### Step 2 — MINT on TEC-1

- Upload `xy_telescope.mint` (comments stripped) to MINT
- Attach logic analyser to TEC-1 TX line, set UART decoder to 4800 baud 8N1
- Run `A` — should see bytes `FX030` then `FX000` on the analyser
- Run `S` — should see `FX000`

### Step 3 — End to end

- T-junction TEC-1 TX to ATtiny PB3 and PC terminal
- Confirm MINT console still works normally
- Run `A`, `S`, `30 100 X` with motors (or LEDs) connected

### Minimal hardware rig

| Item | Purpose |
|------|---------|
| ATtiny85 + USBasp | Flash firmware |
| USB-serial adapter (CP2102 / CH340) | Step 1 testing, bypass TEC-1 |
| L293D or L298N | H-bridge |
| 2× small DC motors (or LEDs + resistors) | Output verification |
| Logic analyser (LA104 etc.) | Validate serial timing and PWM |
| 6–12V motor supply | Separate from logic 5V |

---

## Design notes

- The ATtiny85 does no computation — it is a pure PWM coprocessor.
  The TEC-1 decides when and how fast to move; the ATtiny just executes.
- Commands are filtered: only `F`, `R`, `S` trigger parsing. All other
  bytes are silently dropped — normal MINT console output on the same
  wire will not accidentally drive the motors.
- Always stop (`H` / `J` / `S`) before reversing direction to avoid
  gear jerk and H-bridge shoot-through.
- If the TEC-1 crashes, the ATtiny holds the last speed indefinitely.
  Add a watchdog timeout if leaving the telescope unattended.

---

## Future expansion — position sensing

The current system is open-loop: you command a speed and duration but have
no feedback on where the telescope is actually pointing. Adding angle sensors
closes the loop and enables goto and tracking.

### Recommended sensors

| Axis | Sensor | Interface | Notes |
|------|--------|-----------|-------|
| Altitude (Y) | MPU-6050 IMU | I2C | Absolute tilt from gravity, <0.5° accuracy, no moving parts |
| Azimuth (X) | AS5600 magnetic encoder | I2C | 12-bit absolute angle (0.09° resolution), small magnet on shaft |

The MPU-6050 is the easiest starting point — one chip, absolute angle, no
reference or homing needed. Add the AS5600 for azimuth once altitude is working.

Avoid a compass (HMC5883L) for azimuth — the motors are too close and will
corrupt the reading.

### Architecture — ATtiny84A handles both sensors and serial

Combining motor driving and sensor reading needs 8 pins minimum — more than
the ATtiny85's 5. The **ATtiny84A** is the natural upgrade: same family, same
toolchain, DIP-14, ~$1-2, and 12 I/O pins.

The MPU-6050 (0x68) and AS5600 (0x36) have different I2C addresses so they
share the same two-wire bus with no conflict.

```
ATtiny84A pin map (combined motor + sensor MCU)
──────────────────────────────────────────────────────
PA0   X_FWD   PWM (OC0A)   → H-bridge IN1
PA1   X_REV   PWM           → H-bridge IN2
PA2   Y_FWD   PWM           → H-bridge IN3
PA3   Y_REV   PWM (OC1B)   → H-bridge IN4
PA4   SCL     I2C clock     → MPU-6050 + AS5600 (shared)
PA6   SDA     I2C data      → MPU-6050 + AS5600 (shared)
PB0   RX      software serial ← TEC-1 TX
PB1   TX      software serial → TEC-1 RX
PB2   free
PB3   free
PA5   free
PA7   free
```

Full system wiring:

```
TEC-1 TX ──┬──► PC terminal
            └──► ATtiny84A PB0 RX  (motor commands in)

ATtiny84A PB1 TX ────────────────► TEC-1 RX  (angle data out)

ATtiny84A PA4/PA6 I2C ──┬──► MPU-6050  (altitude — tilt from gravity)
                         └──► AS5600   (azimuth  — magnetic encoder)

ATtiny84A PA0–PA3 PWM ──────────► H-bridge → X and Y motors
```

The ATtiny84A reads both sensors and transmits a MINT-parseable string at 4800 baud
while continuing to service motor commands:

```
A0325 1573
```

`A` prefix, altitude in tenths of degrees (4 digits, zero-padded), space, azimuth in tenths of degrees.
`0325` = 32.5°, `1573` = 157.3°. Tenths fit MINT's 16-bit signed integers (max 32767) for both axes.

**Why the load is manageable:**

At 4800 baud a motor command arrives every ~2ms and takes 1ms to receive,
leaving long idle gaps. I2C sensor reads at 400kHz take under 0.5ms each.
PWM runs in hardware with zero CPU overhead. Read the sensors during idle
gaps — never mid-command — and there is no timing conflict.

**Conversion:**

- AS5600 → 12-bit raw (0–4095) → ×360 ÷ 4096 → decimal degrees → split to d/m/s
- MPU-6050 → raw accelerometer X/Y/Z → tilt angle via 90-entry arctangent
  lookup table in flash. No floating point needed. Table costs 180 bytes.

Use **TinyWireM** for I2C — written for the ATtiny USI peripheral, compatible
with both sensors. ATTinyCore in Arduino IDE supports the ATtiny84A with the
same setup used for the ATtiny85.

### Position-aware control — xy_telescope.mint

The single MINT file includes full position-aware control. Once the ATtiny84A
firmware is running, these commands work immediately after loading `xy_telescope.mint`:

```
I P               read current position and display it
325 1573 K        set target: altitude 32.5°, azimuth 157.3° (tenths)
W                 goto — drives both motors until on target
0 L W             look up star 0 (Sirius) and goto
T                 track — continuous correction, runs until RESET
```

The ATtiny84A sends `A<alt_tenths> <az_tenths>\n` every 500ms (e.g. `A0325 1573\n`).
MINT function `I` waits for the `A` prefix, parses both 4-digit values, and stores
them in `a` (altitude tenths) and `z` (azimuth tenths). `325` = 32.5°, `1573` = 157.3°.

The goto loop (`W`) reads position, checks whether both axes are within the
threshold `e` (default 2°), drives the correction step if not, and repeats.
Motors stop automatically on arrival.

Tracking (`T`) runs the same correction at slow speed (20) in an infinite
loop. It keeps the telescope pointed at the target as the sky moves. Press
RESET to exit.

The star table holds 5 objects by index (0–4). Edit the `o` array at the top
of `xy_telescope.mint` with coordinates for your location and observation time.

---

## Known limitations and further work

### What works well

**Passive sniffing** — T-junctioning the TEC-1 console TX adds zero hardware
to the Z80 side. The ATtiny sits silently on the line and picks out motor
commands without interrupting anything else running on the TEC-1.

**MINT for high-level logic** — star tables, tracking, and goto are all in
MINT 2, which means you can modify and reload them at the MINT prompt without
reflashing any hardware. This is the right place for that code.

**ATtiny84A scope** — combining motor control and sensor reading in one chip is
the correct call. The 84A has enough I/O for I2C, two serial lines, and four
PWM channels simultaneously.

**32kHz PWM** — pushing the timer prescaler to eliminate audible motor hum is
the kind of detail that separates a usable instrument from an annoying one.

---

### Issues — in order of importance

**1. No watchdog / motor kill** *(implemented)*
`lastSerialMs` is reset every time a byte arrives in `handleSerial()`. In
`loop()`, if any motor is running and `millis() - lastSerialMs > WATCHDOG_MS`
(5 seconds), `stopAll()` is called. Set `WATCHDOG_MS` in the firmware
constants to adjust the timeout.

**2. Backlash compensation** *(implemented — disabled by default)*
`driveMotor()` now fires a short pulse in the new direction before applying
commanded speed whenever a direction change is detected on a running axis.
Controlled by `BACKLASH_X_MS` and `BACKLASH_Y_MS` (both default 0 = disabled).
To enable: run the telescope, command a full-speed slew, then reverse — measure
how long after the reversal command the load actually starts moving. Set that
duration in ms for each axis. `BACKLASH_SPEED` (default 60) is the PWM during
the pulse; tune it to match your gearbox resistance.

**3. Zenith singularity** *(implemented)*
`Q` guards against gimbal lock: `a 85 > ( H ) /E (... normal azimuth correction ...)`.
When altitude exceeds 85°, azimuth motor stops and returns. Below 85° the two-speed
correction runs unchanged.

**4. Bang-bang goto overshoot** *(implemented — two-speed goto)*
`O` and `Q` now use two speeds. When error exceeds `b` (10°), the motor runs
at `f` (fast, 80). When error is between `b` and `e` (2°), it drops to `s`
(slow, 20). Below `e` it stops. `T` (track) sets both `f` and `s` to 20 so
it never triggers the fast zone. Tune `b`, `f`, and `s` at the MINT prompt
for your mount.

**5. Acceleration ramping** *(mechanical — implemented in firmware)*
Full-speed slewing to a dead stop causes mechanical shock and overshoot.
`driveMotor()` now ramps down when speed 0 is commanded and current speed
exceeds `RAMP_STEP` (20). Each step reduces PWM by 20 counts and waits 8ms,
giving ~100ms total at full speed. Normal stops (H, J) use the ramp;
emergency stop (S) remains immediate. Tune `RAMP_STEP` and `RAMP_MS` in
the firmware for your gearbox.

**6. Serial buffer overrun** *(reliability — low probability, monitor first)*
SoftwareSerial has a small buffer. A long burst of non-motor console output
(HELP listing etc.) at 4800 baud could in theory swallow a motor command.
Unlikely with MINT's slow echo rate. Monitor for missed commands; fix only if
observed.

**7. Fixed-point precision** *(implemented — 0.1° resolution)*
The serial format now transmits tenths of degrees (4-digit zero-padded). MINT
variables `a`, `z`, `p`, `q` and the star table all use tenths. The AS5600
is accurate to ~0.1° so this uses its full resolution. The MPU-6050 is
hardware-limited to ~±0.5° regardless of format. For astrophotography-grade
altitude sensing, replace the MPU-6050 with a second AS5600 encoder on the
altitude axis.

**8. Sidereal tracking with RTC** *(enhancement — turns it into a proper instrument)*
The current `T` function corrects toward a fixed star-table position. A DS3231
RTC on the I2C bus (same two wires, address 0x68 — check for clash with
MPU-6050 at 0x68, use DS1307 at 0x50 instead) would let the TEC-1 calculate
hour angle from current time and derive correct sidereal motor rates for each
axis, making tracking work any night without updating the star table.
