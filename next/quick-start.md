# TEC-SCOPE Quick Start Guide

## Complete setup guide from hardware to first light

---

## Phase 1: Hardware Assembly (Week 1)

### Day 1: Gather Components
- [ ] 2x ATtiny85 chips
- [ ] 2x HN3806-AB encoders (you have these)
- [ ] 2x L298N motor driver modules
- [ ] 2x DC geared motors (you have worm drive motor)
- [ ] Power supplies (12V for motors, 5V for logic)
- [ ] Resistors, capacitors (see wiring-diagram.md BOM)
- [ ] Breadboard or prototyping board
- [ ] USBasp or Arduino as ISP programmer (for ATtiny85)

### Day 2: Build Power Supply
1. Connect 12V power supply (2A minimum)
2. Build 5V regulator circuit using 7805
3. Add filtering capacitors (100µF on input/output)
4. Test: Verify 5V output with multimeter
5. Add heatsink to 7805 if it gets warm

### Day 3: Program ATtiny85 Chips

**Required Software:**
- Arduino IDE
- ATtinyCore board support
- USBasp programmer or Arduino as ISP

**Steps:**
1. Install ATtinyCore in Arduino IDE
2. Open `ATtiny85-QEI.ino` from your project
3. **For Chip U2 (Alt encoder):**
   - Uncomment `#define CHIP_1`
   - Select Board: ATtiny25/45/85
   - Select Chip: ATtiny85
   - Select Clock: 8MHz Internal
   - Upload using programmer
4. **For Chip U5 (Az encoder):**
   - Comment out `#define CHIP_1`
   - Upload using programmer
5. Test: Use Arduino serial monitor at 9600 baud to verify chips respond

### Day 4: Build Encoder Interface Circuit

**For EACH encoder:**
1. Wire ATtiny85 to breadboard
2. Add 10kΩ pull-up resistors:
   - PB0 (Enc A) to +5V
   - PB1 (Enc B) to +5V
   - PB5 (RESET) to +5V
3. Add 100nF decoupling capacitor between VCC and GND
4. Connect encoder wires:
   - Red to +5V via 100Ω resistor
   - Black to GND
   - Green (A) to ATtiny PB0
   - White (B) to ATtiny PB1

**Connect SPI bus:**
1. Both ATtiny PB3 to common SPI CLK line
2. Both ATtiny PB4 to common SPI MISO (via 1kΩ resistors each)
3. U2 PB2 to CS1 line
4. U5 PB2 to CS2 line

### Day 5: Build Motor Driver Circuits

**For EACH motor:**
1. Connect L298N to breadboard
2. Wire power:
   - +12V to L298N +12V pin
   - GND to common ground
3. Wire motor:
   - Motor + to OUT1 (or OUT3 for motor 2)
   - Motor - to OUT2 (or OUT4 for motor 2)
4. Wire direction control:
   - IN1 (or IN3) to Z80 port 04h bit 0 (or bit 1)
   - IN2 (or IN4) to GND
5. Wire PWM enable:
   - Option A: ENA/ENB directly to Z80 port 05h/06h (if L298N accepts 5V TTL)
   - Option B: Use 2N2222 transistor buffer (see wiring-diagram.md)

### Day 6-7: Integration and Testing

**Physical setup:**
1. Mount encoders on motor output shafts
2. Verify encoders rotate with motors
3. Mount motors on telescope mount (Alt and Az axes)
4. Connect all wiring, verify with multimeter before power-on

---

## Phase 2: Software Testing (Week 2)

### Test 1: Motor Test
```z80
; Load test-program.z80 into TEC-1 at 0800h
JP TEST_MOTORS
```

**Expected result:**
- Motor 1 spins forward 2 seconds
- Motor 1 spins reverse 2 seconds
- Motor 2 spins forward 2 seconds
- Motor 2 spins reverse 2 seconds

**If it fails:**
- Check L298N wiring
- Verify 12V power supply
- Check direction bits on port 04h with multimeter

### Test 2: SPI Clock Test
```z80
JP TEST_SPI_CLOCK
```

**Expected result:**
- Use oscilloscope on port 03h bit 0
- Should see ~100 clock pulses
- Square wave, ~10µs period at 4MHz Z80

**If it fails:**
- Check Z80 port 03h output
- Verify SPI_PORT is correctly defined (03h)

### Test 3: Read Encoder Test
```z80
JP TEST_READ_ENCODER
```

**Expected result:**
- TEC display shows encoder count (LSB)
- Rotate encoder by hand, value changes
- Clockwise rotation increases count
- Counter-clockwise decreases count

**If it fails:**
- Check ATtiny85 programming (run Arduino serial test)
- Verify SPI wiring (CLK, MISO, CS)
- Check encoder power and A/B connections
- Use scope to verify quadrature signals on ATtiny PB0/PB1

### Test 4: PWM Test
```z80
JP TEST_PWM
```

**Expected result:**
- Use oscilloscope on port 05h
- Should see 50% duty cycle square wave
- Motor 1 should spin at half speed

**If it fails:**
- Check port 05h output
- Verify PWM wiring to L298N ENA
- Check if transistor buffer is needed

### Test 5: Encoder Direction Test
```z80
JP TEST_ENCODER_DIRECTION
```

**Expected result:**
- TEC display shows 01h (correct direction)
- Motor spins, encoder counts up

**If shows FFh (wrong direction):**
- Option A: Swap encoder A/B wires
- Option B: Invert direction bit in code

---

## Phase 3: Main Control Software (Week 3)

### Day 1: Load Main Program
```z80
; Assemble motor-control.z80
; Load binary to TEC-1 at 0800h
JP 0800h  ; Start main program
```

### Day 2: Manual Control Mode
Modify main loop to use joystick:
```z80
MAIN_LOOP:
        CALL READ_ENCODERS
        CALL MANUAL_CONTROL     ; Use this instead of UPDATE_MOTORS
        CALL PWM_CYCLE
        JP MAIN_LOOP
```

Wire buttons to port 07h:
- Bit 0: Alt up
- Bit 1: Alt down
- Bit 2: Az left
- Bit 3: Az right

Test manual slewing in all directions.

### Day 3: Calibration

**Step 3a: Find Zero Position**
1. Point telescope at known star (e.g., Polaris for Az=0)
2. Read encoder positions
3. Save as reference zero

**Step 3b: Measure Counts per Degree**
1. From zero, rotate Alt exactly 10 degrees (use inclinometer)
2. Read encoder position difference
3. Calculate: counts_per_degree = difference / 10
4. Expected: ~16.67 counts/degree (6000 counts / 360°)

**Step 3c: Calibrate Proportional Gain**
Start with gain = 4 (two SLA instructions in UPDATE_MOTORS):
- If overshoots target: Reduce gain (use one SLA = gain 2)
- If too slow: Increase gain (three SLA = gain 8)
- Goal: Reaches target quickly without oscillation

### Day 4: Automatic Position Control

Set target positions:
```assembly
; Point to Az=180°, Alt=45°
; For 600 PPR encoder × 10:1 gear = 6000 counts/rev
; 180° = 3000 counts
; 45° = 750 counts

LD BC, 0000h
LD DE, 0BB8h    ; 3000 decimal = Az target
CALL SET_TARGET2

LD BC, 0000h
LD DE, 02EEh    ; 750 decimal = Alt target
CALL SET_TARGET1

; Main loop will automatically move to target
```

Test GoTo functionality:
- [ ] Moves to target position
- [ ] Stops at target (PWM = 0)
- [ ] Doesn't oscillate excessively

### Day 5: Add Display Output

Modify code to display current position on 7-segment display:
```assembly
; In main loop, add:
CALL DISPLAY_POSITION

DISPLAY_POSITION:
        ; Read Alt position LSB
        LD A, (ENC1_POS+3)
        ; Convert to degrees (simplified)
        ; Display on TEC-1 7-segment
        OUT (01h), A    ; TEC-1 data port
        RET
```

---

## Phase 4: Advanced Features (Week 4+)

### Tracking Mode
Implement sidereal tracking (15.041°/hour):
```assembly
; Every ~240ms, increment Az target by 1 count
; This gives ~15°/hour tracking rate
```

### Stellarium Integration
Add serial output (4800 baud) on spare I/O port:
- Send current position to Stellarium
- Receive GoTo commands from Stellarium

### PID Control
Upgrade from simple P (proportional) to full PID:
- Add integral term (sum of past errors)
- Add derivative term (rate of change of error)
- Smoother, more accurate positioning

### Acceleration/Deceleration
Add motion profiling:
- Start slow, ramp up to max speed
- Slow down as approaching target
- Prevents jerky movements and overshoot

---

## Troubleshooting Guide

### Motors run but telescope doesn't move
- **Gear ratio too high**: Increase PWM duty cycle
- **Mechanical binding**: Check mount for friction points
- **Motor stalled**: Check current draw, may need bigger motor

### Encoders count erratically
- **Electrical noise**: Add capacitors (100nF) across encoder A/B pins
- **Loose connections**: Check encoder wiring
- **Interference**: Route encoder wires away from motor power

### Position drifts over time
- **Worm gear backlash**: Implement anti-backlash algorithm
- **Encoder slipping**: Secure encoder coupling to shaft
- **Lost counts**: Check for noise on SPI bus

### Motors oscillate at target
- **Gain too high**: Reduce proportional gain
- **No deadband**: Add position tolerance (±2 counts)
- **Need damping**: Add derivative term (PD control)

### One axis works, other doesn't
- **Check CS lines**: Verify CS1 and CS2 are separate
- **Swap encoders**: Test if problem follows encoder or motor
- **Check ATtiny programming**: Verify chip IDs are correct

---

## Performance Targets

### Basic Operation (End of Week 2)
- [x] Motors respond to commands
- [x] Encoders read correctly
- [x] Manual slewing works
- [x] Direction control works

### Functional Telescope (End of Week 3)
- [ ] Automatic GoTo positioning
- [ ] Position accuracy: ±0.5°
- [ ] Smooth movement without vibration
- [ ] Position displayed on TEC-1

### Advanced Operation (Week 4+)
- [ ] Sidereal tracking
- [ ] Position accuracy: ±0.1° (6 arc-minutes)
- [ ] Stellarium integration
- [ ] Saved object database

---

## Next Steps After Motor Control

1. **Add limit switches** - Prevent mechanical damage
2. **Implement autoguiding** - For astrophotography
3. **Add web interface** - Use ESP32 for wireless control
4. **Star alignment routine** - 2-star or 3-star alignment
5. **Object catalog** - Store and recall favorite targets
6. **Image capture integration** - Connect with astro/ scripts

---

## Safety Checklist

Before first power-on:
- [ ] All grounds connected together
- [ ] No short circuits (test with multimeter)
- [ ] Correct voltage on all power rails
- [ ] Motors can't damage telescope mechanically
- [ ] Emergency stop button accessible
- [ ] Current limiting on motor power supply
- [ ] Fuses installed

During testing:
- [ ] Start with low PWM values (30-50)
- [ ] Test each axis independently
- [ ] Have hand on emergency stop
- [ ] Monitor motor temperature
- [ ] Check for loose wiring

---

## Resources

**Project Files:**
- `motor-control.z80` - Main control program
- `test-program.z80` - Hardware test routines
- `wiring-diagram.md` - Complete wiring guide
- `motor-control-usage.md` - Detailed API documentation

**External References:**
- ATtiny85 datasheet
- L298N datasheet
- HN3806 encoder datasheet
- OnStep telescope controller (for ideas)
- Stellarium LX200 protocol documentation

**Tools Needed:**
- Z80 assembler (z80asm, pasmo, or similar)
- Arduino IDE (for ATtiny programming)
- Oscilloscope or logic analyzer (highly recommended)
- Multimeter
- Soldering iron
- Wire strippers

---

## Success Criteria

You'll know it's working when:
1. ✅ You can press a button, motor moves telescope
2. ✅ Display shows telescope position in degrees
3. ✅ You set a target, telescope moves there automatically
4. ✅ Telescope tracks a star as it moves across the sky
5. ✅ You take a sharp long-exposure photo of a galaxy

**Welcome to computerized astronomy!** 🔭✨
