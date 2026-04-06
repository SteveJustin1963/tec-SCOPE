# DC Motors vs Stepper Motors for Telescope Control

## Quick Answer: Use DC Motors with PWM + Encoders

For your telescope project, DC motors with PWM control and encoders is the superior choice.

## Detailed Comparison

### DC Motors with PWM (RECOMMENDED)

**Advantages:**
1. **Smoother motion** - Critical for tracking stars without vibration
2. **Quieter operation** - Steppers buzz and vibrate (bad for imaging!)
3. **Better for slow tracking** - Can achieve smooth sidereal tracking speeds
4. **You already have the hardware** - Worm drive motor + encoders = perfect combo
5. **Simpler Z80 control** - Just PWM (easy to bit-bang) + direction pin
6. **Lower cost** - L298N driver ~$3 vs stepper driver ~$10-15
7. **Higher torque at all speeds** - Better for slewing and tracking
8. **Closed-loop control** - Encoders provide position feedback
9. **No missed steps** - Encoders tell you actual position
10. **Worm gear benefits** - Self-locking, won't drift when powered off

**Disadvantages:**
1. Need encoders for position feedback (but you already have these!)
2. Slightly more complex control algorithm (but MINT code already exists)

**Z80 Implementation:**
```
Simple PWM from Z80:
- 2 GPIO pins per motor (PWM + Direction)
- Bit-bang PWM in software loop
- Read encoder position via SPI (ATtiny85)
- Adjust PWM based on position error
```

---

### Stepper Motors (NOT RECOMMENDED)

**Advantages:**
1. Precise positioning without encoders (open-loop)
2. Hold torque when stationary

**Disadvantages:**
1. **Noisy and vibrates** - Ruins astrophotography images!
2. **Resonance issues** - Certain speeds cause severe vibration
3. **Can miss steps** - Especially under load or vibration (no feedback!)
4. **Poor slow-speed performance** - Difficult to achieve smooth tracking
5. **More expensive** - Stepper drivers cost more
6. **Complex Z80 control** - Need to generate step pulses with precise timing
7. **Microstepping needed** - For smooth motion, adds complexity
8. **Higher power consumption** - Always drawing current
9. **Lower torque at high speeds** - Bad for fast slewing
10. **Your encoders would be wasted** - Already have position feedback!

**Z80 Implementation Challenge:**
```
Stepper complexity:
- Need 2-4 pins per motor (depending on driver)
- Must generate precise step pulse timing
- Acceleration/deceleration ramps required
- Microstepping = even more complex waveforms
- Hard to do from Z80 without dedicated controller
```

---

## Why DC Motors Win for Telescopes

### 1. Tracking Smoothness
Commercial telescope mounts (even $10,000+ ones) often use DC motors with encoders because:
- Tracking stars requires ultra-smooth motion at 15°/hour (0.0042°/second)
- Stepper "cogging" creates vibration that blurs images
- DC motors with worm gears give buttery-smooth tracking

### 2. You Already Have the Right Hardware
- Worm drive motor (from foot massage machine) = perfect for telescope
- HN3806-AB encoders (600 PPR × 10:1 gearing = 6000 counts/rev)
- Resolution: 360° / 6000 = 0.06° per count = 3.6 arc-minutes
- That's excellent resolution for visual use!

### 3. Simpler Z80 Code
**DC Motor PWM (Easy):**
```assembly
; Simple PWM loop
MOTOR_CONTROL:
    LD A, (PWM_DUTY)    ; Get duty cycle 0-255
    OUT (MOTOR_PORT), A  ; Set motor speed
    LD A, (DIRECTION)
    OUT (DIR_PORT), A    ; Set direction
    RET
```

**Stepper Control (Complex):**
```assembly
; Must maintain phase sequence, timing, acceleration...
; Much more complex, needs interrupt-driven stepping
; Or use external stepper controller (defeats purpose)
```

### 4. Cost Comparison
| Component | DC Motor Solution | Stepper Solution |
|-----------|------------------|------------------|
| Motor | $0 (you have it) | $20-40 (×2) |
| Driver | L298N: $3 (×2) = $6 | A4988: $10 (×2) = $20 |
| Encoder | $15 (×2) = $30 | Optional (wasted) |
| **Total** | **$36** | **$60-100** |

---

## Recommended DC Motor Setup

### Hardware Configuration
```
Alt Axis:
  Worm Drive Motor (24V from foot massager)
  → Run at 12V with PWM for speed control
  → L298N H-Bridge driver
  → HN3806-AB encoder on output shaft
  → ATtiny85 for encoder counting

Az Axis:
  DC Geared Motor (need to source if not available)
  → 12V with PWM control
  → L298N H-Bridge driver
  → HN3806-AB encoder on output shaft
  → ATtiny85 for encoder counting
```

### Control Loop (Closed-Loop Position Control)
```
1. Read target position (from user or Stellarium)
2. Read actual position (from encoder via SPI)
3. Calculate error (target - actual)
4. Set PWM duty cycle proportional to error (PID control)
5. Set direction based on error sign
6. Repeat at ~50-100 Hz
```

### Simple P Controller (Start Here)
```mint
// Simplified MINT pseudocode
:M  // Motor control loop
  T S - E!        // Error = Target - Current position from SPI
  E 0 < (         // If error < 0
    0 D!          // Direction = 0 (reverse)
    E -1 * E!     // Error = abs(error)
  ) /E (
    1 D!          // Direction = 1 (forward)
  )
  E 10 * P!       // PWM = Error * 10 (proportional gain)
  P 255 > ( 255 P! )  // Limit PWM to 255 max
  P #42 /O        // Output PWM to port 42h
  D #43 /O        // Output direction to port 43h
  /M              // Loop
;
```

---

## Real-World Telescope Mounts Use DC Motors

Professional and prosumer mounts use DC servos:
- **iOptron CEM series** - DC servos with encoders
- **Sky-Watcher EQ6-R Pro** - DC servos with encoders
- **Celestron CGX** - DC servos with encoders
- **Software Bisque Paramount** - DC servos with encoders

Steppers are mainly used in:
- Very cheap beginner mounts (because open-loop is simpler for manufacturers)
- 3D printers (where noise doesn't matter and precision is needed)
- CNC machines (rigidly mounted, vibration less critical)

---

## Final Recommendation

**Use your DC motors with:**
1. PWM speed control (easy from Z80)
2. HN3806-AB encoders (you already have these)
3. L298N H-bridge drivers ($3 each)
4. ATtiny85 for encoder counting (code already exists)
5. Closed-loop position control (simple P or PID algorithm)

This gives you:
- Professional-quality smooth tracking
- Precise positioning with encoder feedback
- Low cost ($36 total for drivers + encoders)
- Simple Z80 implementation
- Quiet operation for imaging
- Uses hardware you already have

**Don't use steppers** - they'll cost more, vibrate, make noise, and give worse performance for telescope control.
