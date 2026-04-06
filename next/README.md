# Next Steps - Adding Two Geared Motors to Telescope

## 📋 Quick Navigation

**IMPORTANT: Two Versions Available!**
- ⭐ **[VERSION-COMPARISON.md](VERSION-COMPARISON.md)** - Which version should you build?

### Version 1: Advanced (Quadrature Encoders + SPI)
- **[motor-control.z80](motor-control.z80)** - Main code with SPI encoders (~700 bytes)
- **[wiring-diagram.md](wiring-diagram.md)** - Complete wiring for encoder version
- **[test-program.z80](test-program.z80)** - Hardware tests for encoders
- **[quick-start.md](quick-start.md)** - Week-by-week build guide
- **Cost:** ~$60 | **Resolution:** 0.06° | **Best for:** Astrophotography

### Version 2: Simple (Optical Gate Sensors) ⭐ RECOMMENDED FOR BEGINNERS
- **[optical-gate-control.z80](optical-gate-control.z80)** - Main code with optical sensors (~500 bytes)
- **[optical-wiring.md](optical-wiring.md)** - Complete wiring for optical version
- **[optical-test.z80](optical-test.z80)** - Hardware tests for optical sensors
- **[optical-gate-simple.md](optical-gate-simple.md)** - How to build slotted disk & sensors
- **Cost:** ~$25 | **Resolution:** 0.45° | **Best for:** Visual observing

### General Files
- **[motor-comparison.md](motor-comparison.md)** - Why use DC motors instead of steppers

---

## Summary: What You're Building

### Option 1: Advanced Encoder Version
A closed-loop DC motor control system using:
- **2x DC motors** with PWM speed control (you have worm drive motor)
- **2x HN3806-AB encoders** for position feedback (600 PPR × 10:1 = 6000 counts/rev)
- **2x ATtiny85** chips to handle encoder counting via SPI
- **2x L298N** motor driver modules for bi-directional control
- **Z80 assembly code** running on your TEC-1 SBC at 0800h (fits in 2K RAM)
- **Result:** GoTo telescope with 0.06° resolution (~3.6 arc-minutes per count)
- **Cost:** ~$60

### Option 2: Simple Optical Gate Version ⭐ EASIER!
A simple DC motor control system using:
- **2x DC motors** with PWM speed control (100:1 gear reduction you mentioned!)
- **2x Optical slot sensors** for position feedback (DIY slotted disk with 8 slots)
- **2x L298N** motor driver modules for bi-directional control
- **Z80 assembly code** running on your TEC-1 SBC at 0800h (fits in 2K RAM)
- **No microcontroller programming needed!**
- **Result:** GoTo telescope with 0.45° resolution (~27 arc-minutes per count)
- **Cost:** ~$25 (saves $35!)

---

Based on your project documentation, you're at an exciting stage! You have two geared motors ready to add to your telescope. Here are the next steps:

## Immediate Next Steps

### 1. Motor Mounting & Gearing
**Altitude (Alt) Axis:**
- Mount your worm drive motor (from foot massage machine or similar)
- Gear ratio target: 10:1 or higher for smooth tracking
- Need to run at lower voltage with PWM control (you mentioned 24V motor needs reduction)

**Azimuth (Az) Axis:**
- Need a compact motor solution (README.md:52 mentions space constraints under mount)
- Consider belt drive or friction drive if worm drive won't fit

### 2. Encoder Installation
Install your HN3806-AB encoders (600 pulses/rev) on both axes:
- Wire with pull-up resistors (don't connect A/B directly to VCC!)
- Green=A phase, White=B phase, Red=Vcc+, Black=V0
- These will give you position feedback

### 3. Motor Driver Electronics
You need H-bridge drivers for bi-directional control:
- Options: L298N modules (~$3 each), or discrete MOSFETs
- Need PWM control for speed regulation
- Two drivers (one per motor)

### 4. Encoder Interface Decision
**Choose one approach:**

**Option A: ATtiny85/84 (Recommended - $1 each):**
- Use two ATtiny85s as SPI slaves
- Code already exists: ATtiny84-QEI.ino and ATtiny85-QEI.ino
- Handles quadrature decoding in hardware
- Cheaper and easier than LS7366R

**Option B: LS7366R ($30+ each):**
- More expensive, harder to source
- Code exists: LS7366R_9511_ SPI.mint

### 5. Control System Integration
Wire everything to your TEC-1/Z80 system:
- SPI bus for encoder readings
- GPIO for motor PWM control
- Test with main_loop1.mint code framework

## Suggested Order of Implementation

1. **Physical mounting first** - Get motors mechanically attached
2. **Test motors alone** - Verify they work with basic PWM before integration
3. **Add encoders** - Mount and wire encoders
4. **Program ATtiny chips** - Flash the quadrature decoder code
5. **Test SPI communication** - Verify you can read encoder counts
6. **Integrate motor control** - Add PWM control to your Z80 code
7. **Basic slewing** - Manual joystick control
8. **Position tracking** - Display coordinates on 7-segment display

## Detailed Implementation Tasks

### Task 1: Motor Driver Circuit
Create H-bridge motor driver for each motor:
- L298N module (easier) or discrete MOSFET H-bridge
- PWM input from Z80 I/O port
- Direction control from Z80 I/O port
- Power supply: appropriate voltage for your motors

### Task 2: Encoder Wiring
For each HN3806-AB encoder:
```
Encoder Pin -> Destination
Red (Vcc)   -> +5V via protection
Black (V0)  -> GND
Green (A)   -> ATtiny85 PB0 (with pull-up resistor)
White (B)   -> ATtiny85 PB1 (with pull-up resistor)
```

### Task 3: ATtiny85 Programming
- Compile ATtiny85-QEI.ino with CHIP_1 defined for first encoder (U2)
- Compile ATtiny85-QEI.ino with CHIP_1 commented out for second encoder (U5)
- Flash each chip accordingly
- Wire SPI bus: CLK (PB3), MISO (PB4), separate CS lines

### Task 4: SPI Bus Architecture
```
Z80 TEC-1 Master
    |
    +-- SPI CLK  --> Both ATtiny85s
    +-- SPI MISO <-- Both ATtiny85s
    +-- CS1      --> ATtiny85 U2 (Alt encoder)
    +-- CS2      --> ATtiny85 U5 (Az encoder)
```

### Task 5: Z80 Code Development
Based on existing code (generic_SPI.z80, compact_SPI.z80):
1. Initialize SPI interface
2. Read encoder positions via SPI
3. Convert counts to angles
4. Output PWM for motor control
5. Implement simple slewing commands
6. Add joystick input for manual control

### Task 6: Display System
Seven segment display (6 digits):
- Press A: Show Az and Alt together (e.g., "0299 23" = Az=299°, Alt=23°)
- Press B: Show Az in deg-min (e.g., "0299 44" = 299° 44')
- Press C: Show Alt in deg-min (e.g., "0023 51" = 23° 51')

## Hardware Shopping List

- [ ] 2x ATtiny85 or 1x ATtiny84 (~$1-2 each)
- [ ] 2x L298N motor driver modules (~$3 each) OR discrete MOSFETs
- [ ] Pull-up resistors for encoder signals (4x 10kΩ)
- [ ] Decoupling capacitors (100nF ceramic for each IC)
- [ ] Power supply for motors (voltage depends on your motors)
- [ ] Voltage regulator if needed (7805 for 5V logic from higher voltage)
- [ ] Prototyping board or PCB
- [ ] Connectors for motors and encoders

## Testing Checklist

### Phase 1: Motors Only
- [ ] Motor 1 spins forward on command
- [ ] Motor 1 spins reverse on command
- [ ] Motor 1 speed varies with PWM
- [ ] Motor 2 spins forward on command
- [ ] Motor 2 spins reverse on command
- [ ] Motor 2 speed varies with PWM

### Phase 2: Encoders Only
- [ ] ATtiny85 #1 programmed successfully
- [ ] ATtiny85 #2 programmed successfully
- [ ] SPI communication with chip #1 working
- [ ] SPI communication with chip #2 working
- [ ] Encoder #1 counts increment when rotated CW
- [ ] Encoder #1 counts decrement when rotated CCW
- [ ] Encoder #2 counts increment when rotated CW
- [ ] Encoder #2 counts decrement when rotated CCW

### Phase 3: Integration
- [ ] Read encoder position from Z80
- [ ] Display encoder counts on 7-segment display
- [ ] Control motor from Z80 GPIO
- [ ] Manual slewing with joystick works
- [ ] Position tracking during motor movement
- [ ] Motors stop at commanded positions

### Phase 4: Calibration
- [ ] Set reference position (known star or North)
- [ ] Verify encoder counts match actual angles
- [ ] Test gear ratio calculations (10:1 reduction)
- [ ] Verify 600 pulses/rev * 10 gear ratio = 6000 counts/revolution
- [ ] Calculate degrees per count: 360° / 6000 = 0.06° per count

## Code Files to Study

1. **ATtiny84-QEI.ino** - Encoder interface for ATtiny84
2. **ATtiny85-QEI.ino** - Encoder interface for ATtiny85
3. **generic_SPI.z80** - Generic SPI communication routines
4. **compact_SPI.z80** - Compact SPI implementation
5. **main_loop1.mint** - Main control loop framework
6. **DGEPS1.mint** - Dual Geared Encoder Position System
7. **QETBMS1.mint** - Quadrature Encoder Test with Motion Simulation

## Future Enhancements (After Basic Motor Control Works)

- Stellarium integration via serial (4800 baud)
- Autoguiding for long exposures
- GoTo functionality with star database
- Tracking mode for astrophotography
- Web interface using ESP32/Arduino as coprocessor
- Integration with astro/ scripts for automated imaging sessions

## Safety Notes

- Add limit switches to prevent mechanical damage
- Implement software limits in code
- Use fuses on motor power supplies
- Add emergency stop button
- Test at slow speeds first before increasing PWM
- Ensure motors can't run indefinitely (timeout protection)

## Resources

- ATtiny Programming: Use Arduino as ISP or USBasp programmer
- SPI Testing: Use logic analyzer or oscilloscope to verify signals
- Motor Testing: Test with separate power supply before Z80 integration
- Calibration: Use smartphone compass app for initial azimuth calibration

---

## 💰 Cost Breakdown

### Version 1: Encoder System (Advanced)
| Item | Quantity | Cost | Total |
|------|----------|------|-------|
| ATtiny85 | 2 | $1 | $2 |
| HN3806-AB Encoder | 2 | $15 | $30 |
| L298N Driver | 2 | $3 | $6 |
| USBasp Programmer | 1 | $10 | $10 |
| 12V Power Supply (2A) | 1 | $10 | $10 |
| 7805 Regulator | 1 | $1 | $1 |
| Resistors/Capacitors | - | - | $2 |
| Breadboard/PCB | 1 | $5 | $5 |
| Misc wire/connectors | - | - | $5 |
| **Total** | | | **~$61** |

### Version 2: Optical Gate System (Simple) ⭐
| Item | Quantity | Cost | Total |
|------|----------|------|-------|
| Optical Slot Sensor | 2 | $2 | $4 |
| L298N Driver | 2 | $3 | $6 |
| Tin lid (scrap) | 2 | FREE | $0 |
| 12V Power Supply (2A) | 1 | $10 | $10 |
| 7805 Regulator | 1 | $1 | $1 |
| Resistors/Capacitors | - | - | $2 |
| Breadboard/PCB | 1 | $5 | $5 |
| Misc wire/connectors | - | - | $3 |
| **Total** | | | **~$23** |

**💰 SAVINGS: $38 with optical gate version!**

*Note: Both exclude motors (you already have)*

---

## 🎯 Decision Summary

**Question:** Should I use DC motors with PWM or stepper motors?

**Answer:** **DC motors with PWM** (See motor-comparison.md for full analysis)

**Reasons:**
1. Smoother tracking (no stepper vibration/cogging)
2. Quieter operation
3. Better torque at all speeds
4. Simpler Z80 control
5. Lower cost ($6 vs $20+ for steppers)
6. You already have the right hardware
7. Professional mounts use DC servos with encoders

**Stepper motors** would cost more, vibrate during tracking (ruining astrophotography), and be harder to control from Z80.

---

## 📖 How to Use This Folder

### If you're ready to start building:
1. Read **quick-start.md** - Follow week-by-week plan
2. Review **wiring-diagram.md** - Understand all connections
3. Order parts from cost breakdown above
4. Program ATtiny85 chips with existing `.ino` files
5. Build and test hardware using **test-program.z80**
6. Load **motor-control.z80** for full operation

### If you want to understand the design:
1. Read **motor-comparison.md** - Why DC motors?
2. Review **motor-control.z80** - Study the code structure
3. Check **motor-control-usage.md** - See API and examples
4. Review **wiring-diagram.md** - Understand hardware architecture

### If you need help:
- Each `.md` file has detailed troubleshooting sections
- Test programs isolate each subsystem
- Wiring diagram shows all connections clearly
- Usage guide has calibration procedures

---

## ✅ Success Milestones

- [ ] **Week 1:** Hardware assembled, power tested
- [ ] **Week 2:** All tests pass (motors, encoders, SPI, PWM)
- [ ] **Week 3:** Manual control working, automatic GoTo functional
- [ ] **Week 4:** Tracking mode active, Stellarium integration
- [ ] **First Light:** Successfully track and photograph a deep-sky object!

**Good luck with your motorized telescope build!** 🔭✨
