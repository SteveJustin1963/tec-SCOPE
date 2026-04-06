/*
 * xy_telescope.ino
 * ATtiny85 telescope X/Y axis DC motor controller
 *
 * Receives serial commands from a Z80 SBC (or any TTL serial source)
 * and drives two DC motors via an H-bridge (L293D or L298N).
 *
 * Command format:  <DIR><AXIS><NNN>
 *   DIR  : F = forward, R = reverse
 *   AXIS : X or Y
 *   NNN  : speed 000-255 (zero-padded to 3 digits)
 *
 * Examples:
 *   FX100  - X axis forward at speed 100
 *   RX050  - X axis reverse at speed 50
 *   FY255  - Y axis forward, full speed
 *   RY000  - Y axis stop
 *   SX000  - emergency stop all (any axis, S prefix)
 *
 * Baud rate: 4800 (keep low for reliable SoftwareSerial on ATtiny85)
 *
 * Pin assignments (ATtiny85 physical / Arduino pin numbers):
 *   PB0 / Pin 5 (Arduino 0) - X forward PWM  -> H-bridge IN1 (X)
 *   PB1 / Pin 6 (Arduino 1) - X reverse PWM  -> H-bridge IN2 (X)
 *   PB2 / Pin 7 (Arduino 2) - Y reverse PWM  -> H-bridge IN4 (Y)
 *   PB3 / Pin 2 (Arduino 3) - Serial RX       -> Z80 TX (TTL 5V only)
 *   PB4 / Pin 3 (Arduino 4) - Y forward PWM  -> H-bridge IN3 (Y)
 *
 * H-bridge wiring (L293D example):
 *   Enable 1 tied high (or use PWM here for finer control)
 *   IN1 <- PB0 (X_FWD)
 *   IN2 <- PB1 (X_REV)
 *   IN3 <- PB4 (Y_FWD)
 *   IN4 <- PB2 (Y_REV)
 *   OUT1/OUT2 -> X motor
 *   OUT3/OUT4 -> Y motor
 *   Vs (motor supply) separate from Vss (logic 5V)
 *   All grounds tied together
 *
 * IMPORTANT: Z80 TX must be 5V TTL.
 * If using RS-232 levels (+/-12V), add a MAX232 level shifter.
 */

#include <SoftwareSerial.h>

// Motor pins
#define X_FWD 0   // PB0
#define X_REV 1   // PB1
#define Y_REV 2   // PB2
#define Y_FWD 4   // PB4

// Serial RX from Z80
#define RX_PIN 3  // PB3, no TX needed

#define BAUD_RATE 4800
#define DIGIT_TIMEOUT_MS 100
#define NUM_DIGITS 3

// ATTinyCore SoftwareSerial requires a valid TX pin even if unused.
// We assign the reset pin (PB5/pin 5) as dummy TX — never driven.
#define DUMMY_TX 5
SoftwareSerial z80Serial(RX_PIN, DUMMY_TX);  // RX only, TX never called

// ---------------------------------------------------------------------------

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

// Read exactly NUM_DIGITS characters with a timeout.
// Returns true if all digits arrived in time.
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

// ---------------------------------------------------------------------------

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

    // Emergency stop: any 'S' prefix
    if (dir == 'S') {
        stopAll();
        return;
    }

    if (dir != 'F' && dir != 'R') return;  // ignore noise

    // Wait for axis character
    unsigned long t = millis();
    while (!z80Serial.available()) {
        if ((millis() - t) > DIGIT_TIMEOUT_MS) return;
    }
    char axis = z80Serial.read();
    if (axis != 'X' && axis != 'Y') return;

    // Read 3-digit speed value
    char buf[NUM_DIGITS + 1];
    if (!readDigits(buf)) return;

    uint8_t speed = (uint8_t)constrain(atoi(buf), 0, 255);
    driveMotor(dir, axis, speed);
}
