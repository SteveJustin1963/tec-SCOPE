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
 *   PA2  (2)   RX    SoftwareSerial ← TEC-1 TX (motor commands)
 *   PA3  (3)   TX    SoftwareSerial → TEC-1 RX (angle data)
 *   PA6  (6)   Y_REV PWM OC1B       → H-bridge IN4
 *   PA7  (7)   X_FWD PWM OC0B       → H-bridge IN1
 *   PB1  (9)   Y_FWD PWM OC1A       → H-bridge IN3
 *   PB2  (10)  X_REV PWM OC0A       → H-bridge IN2
 *   PA4, PA5, PB0 — spare
 *
 * I2C bus: 4k7 pull-ups on SDA and SCL to 5V
 * I2C devices: MPU-6050 addr 0x68, AS5600 addr 0x36 (no conflict)
 *
 * Mounting note:
 *   MPU-6050 mounted so that az (Z-axis) points up at horizon,
 *   ay (Y-axis) points up at zenith. Adjust read_altitude() if different.
 */

#include <SoftwareSerial.h>

// ── Pin definitions ──────────────────────────────────────────────────────────

#define SDA_PIN  0
#define SCL_PIN  1
#define RX_PIN   2
#define TX_PIN   3

#define X_FWD    7   // PA7 OC0B
#define X_REV   10   // PB2 OC0A
#define Y_FWD    9   // PB1 OC1A
#define Y_REV    6   // PA6 OC1B

// ── Constants ────────────────────────────────────────────────────────────────

#define BAUD             4800
#define DIGIT_TIMEOUT_MS  100
#define NUM_DIGITS          3
#define SENSOR_INTERVAL   500   // ms between angle transmissions
#define RAMP_STEP          20   // PWM counts per ramp step on stop
#define RAMP_MS             8   // ms between ramp steps (~100ms at full speed)
#define WATCHDOG_MS      5000   // stop motors if no serial traffic for this long
#define BACKLASH_X_MS       0   // direction-change pulse duration X axis (ms) — measure empirically
#define BACKLASH_Y_MS       0   // direction-change pulse duration Y axis (ms) — measure empirically
#define BACKLASH_SPEED     60   // PWM speed during backlash pulse

#define MPU6050_ADDR     0x68
#define AS5600_ADDR      0x36

// ── SoftwareSerial ───────────────────────────────────────────────────────────

SoftwareSerial tec1(RX_PIN, TX_PIN);

// ── Bit-bang I2C ─────────────────────────────────────────────────────────────
// Open-drain style: drive LOW to pull down, release to let pull-up take it HIGH

void sda_low()  { pinMode(SDA_PIN, OUTPUT); }
void sda_high() { pinMode(SDA_PIN, INPUT);  }
void scl_low()  { pinMode(SCL_PIN, OUTPUT); }
void scl_high() { pinMode(SCL_PIN, INPUT);  }

void i2c_init() {
    digitalWrite(SDA_PIN, LOW);
    digitalWrite(SCL_PIN, LOW);
    sda_high();
    scl_high();
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

// Returns true if ACK received
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
    scl_low();
    sda_high();
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
    i2c_write_byte(reg);
    i2c_write_byte(val);
    i2c_stop();
    return true;
}

// ── Motor control ─────────────────────────────────────────────────────────────
// Track active speed and direction per axis for ramp-down on stop

static uint8_t x_speed = 0, y_speed = 0;
static bool    x_fwd   = true, y_fwd = true;

// stopAll: immediate hard stop — use for emergency (S command) only
void stopAll() {
    x_speed = 0; y_speed = 0;
    analogWrite(X_FWD, 0); analogWrite(X_REV, 0);
    analogWrite(Y_FWD, 0); analogWrite(Y_REV, 0);
}

// driveMotor: set axis direction and speed
// On stop (speed==0): ramps down if current speed > RAMP_STEP
// On direction change: fires a short backlash pulse before applying new speed
//   BACKLASH_X_MS / BACKLASH_Y_MS default to 0 (disabled) — set after measuring your gearbox
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
// tan_lut[i] = round(tan(i°) * 1000) for i = 0..89
// Used for altitude calculation from MPU-6050 accelerometer ratio

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

// Returns angle in degrees (0-89) from |numerator/denominator| * 1000
// Caller must ensure den != 0 and both values are positive
uint8_t atan_deg(uint16_t ratio) {
    for (uint8_t i = 0; i < 89; i++) {
        uint16_t next = pgm_read_word(&tan_lut[i + 1]);
        if (ratio < next) return i;
    }
    return 89;
}

// ── MPU-6050 ──────────────────────────────────────────────────────────────────

void mpu6050_init() {
    i2c_write_reg(MPU6050_ADDR, 0x6B, 0x00);  // clear sleep bit, wake up
    delay(10);
}

// Returns altitude angle as degrees * 100 (e.g. 3214 = 32.14°), -1 on error
// Sub-degree resolution via interpolation between lookup table entries
int16_t read_altitude_centideg() {
    uint8_t buf[6];
    if (!i2c_read_regs(MPU6050_ADDR, 0x3B, buf, 6)) return -1;

    // Raw 16-bit signed accelerometer values
    int16_t ay = (int16_t)((buf[2] << 8) | buf[3]);
    int16_t az = (int16_t)((buf[4] << 8) | buf[5]);

    // Altitude = atan2(ay, az) — adjust axis order for your mounting
    uint16_t num = (uint16_t)abs(ay);
    uint16_t den = (uint16_t)abs(az);
    if (den == 0) return 9000;  // 90.00°

    // ratio = (num / den) * 1000, using 32-bit to avoid overflow
    uint16_t ratio = (uint16_t)((uint32_t)num * 1000 / den);

    uint8_t deg = atan_deg(ratio);

    // Interpolate for fractional degree
    uint16_t lo = pgm_read_word(&tan_lut[deg]);
    uint16_t hi = (deg < 89) ? pgm_read_word(&tan_lut[deg + 1]) : 10000;
    uint8_t  frac100 = (hi == lo) ? 0 : (uint8_t)((uint32_t)(ratio - lo) * 100 / (hi - lo));

    return (int16_t)deg * 100 + frac100;
}

// ── AS5600 ────────────────────────────────────────────────────────────────────

// Returns azimuth in degrees * 100 (e.g. 15708 = 157.08°), -1 on error
int32_t read_azimuth_centideg() {
    uint8_t buf[2];
    if (!i2c_read_regs(AS5600_ADDR, 0x0E, buf, 2)) return -1;
    uint16_t raw = ((uint16_t)(buf[0] & 0x0F) << 8) | buf[1];  // 12-bit 0-4095
    // raw * 36000 / 4096 = centidegrees, use 32-bit arithmetic
    return (int32_t)raw * 36000L / 4096L;
}

// ── Angle output ──────────────────────────────────────────────────────────────

// Send "A<4dig_alt_tenths> <4dig_az_tenths>\n"
// e.g. "A0325 1573\n" = altitude 32.5 degrees, azimuth 157.3 degrees
// Tenths of a degree: alt 0-900, az 0-3599 — fits MINT 16-bit signed (max 32767)
void transmit_angles(int16_t alt_cd, int32_t az_cd) {
    uint16_t alt_td = (uint16_t)(alt_cd / 10);   // centideg -> tenths: 0-900
    uint16_t az_td  = (uint16_t)(az_cd  / 10);   // centideg -> tenths: 0-3599
    tec1.print('A');
    tec1.print(alt_td / 1000);
    tec1.print((alt_td / 100) % 10);
    tec1.print((alt_td / 10) % 10);
    tec1.print(alt_td % 10);
    tec1.print(' ');
    tec1.print(az_td / 1000);
    tec1.print((az_td / 100) % 10);
    tec1.print((az_td / 10) % 10);
    tec1.print(az_td % 10);
    tec1.print('\n');
}

// ── Setup & main loop ─────────────────────────────────────────────────────────

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

    // Watchdog: if a motor is running and no serial byte has arrived for WATCHDOG_MS,
    // the TEC-1 has likely crashed — stop all motors immediately
    if ((x_speed > 0 || y_speed > 0) && (millis() - lastSerialMs) > WATCHDOG_MS)
        stopAll();

    // Only read sensors when serial line has been idle (no command in progress)
    if ((millis() - lastSensor) >= SENSOR_INTERVAL && !tec1.available()) {
        int16_t alt = read_altitude_centideg();
        int32_t az  = read_azimuth_centideg();
        if (alt >= 0 && az >= 0) transmit_angles(alt, az);
        lastSensor = millis();
    }
}
