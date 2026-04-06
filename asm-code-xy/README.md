# Telescope Stepper Motor Control - Z80 Assembly

**Version 1.1** - Production Ready ✅

Z80 assembly code to control two stepper motors for telescope altitude and azimuth positioning with 10:1 gear reduction. Designed for the TEC-1 Single Board Computer.

## 🚀 Quick Start

**New users start here:** [USER-MANUAL.md](USER-MANUAL.md) 📘

**Find documentation:** [DOCUMENTATION-INDEX.md](DOCUMENTATION-INDEX.md) 📚

**Quick reference:** [QUICK-REFERENCE.md](QUICK-REFERENCE.md) 📋

## 📁 Files

### Source Code
- **config.z80** - Configuration constants and definitions
- **stepper-lib.z80** - Stepper motor library routines
- **keypad-input.z80** - TEC-1 keypad interface module (NMI-based input)
- **main.z80** - Main program with keypad interface and examples
- **simple-test.z80** - Simple standalone test program

### Documentation

#### For Users
- **[USER-MANUAL.md](USER-MANUAL.md)** - ⭐ Complete user guide (START HERE)
- **[QUICK-REFERENCE.md](QUICK-REFERENCE.md)** - Command reference card
- **[KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md)** - Keypad system explained
- **[HARDWARE.md](HARDWARE.md)** - Hardware connections and wiring
- **[DOCUMENTATION-INDEX.md](DOCUMENTATION-INDEX.md)** - Find the right docs

#### For Developers
- **[SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md)** - Architecture overview
- **[IMPLEMENTATION-STATUS.md](IMPLEMENTATION-STATUS.md)** - Status tracking
- **[flow.md](flow.md)** - ASCII flowcharts
- **[tec1-sbc-spec.md](tec1-sbc-spec.md)** - TEC-1 hardware spec

#### Version 1.1 Updates (2026-01-19)
- **[BUG-FIXES.md](BUG-FIXES.md)** - Critical bugs fixed
- **[STEPPING-OPTIONS.md](STEPPING-OPTIONS.md)** - Motor drive modes
- **[CHANGES-SUMMARY.md](CHANGES-SUMMARY.md)** - What changed

### Build System
- **Makefile** - Build automation

## Hardware Requirements

### Stepper Motors
- 2x bipolar stepper motors (typically 200 steps/revolution, 1.8° per step)
- 10:1 gear reduction on each axis
- Motor drivers (L298N, ULN2003, or similar)

### Connections
- **ALT_PORT (01H)** - Altitude motor control (4 bits: coil outputs)
- **AZ_PORT (02H)** - Azimuth motor control (4 bits: coil outputs)
- **STATUS_PORT (03H)** - Optional status/limit switches

### Bit Assignments (for each motor port)
```
Bit 0: Coil A
Bit 1: Coil B
Bit 2: Coil A' (complement)
Bit 3: Coil B' (complement)
```

## System Specifications

### Movement Resolution
- **Base Steps**: 200 steps/revolution (1.8° per step)
- **Gear Ratio**: 10:1
- **Total Steps for 360°**: 2000 steps
- **Resolution**: 0.18° per step (approximately 11 arc-minutes)

### Position Ranges
- **Altitude**: 0° to 90° (0 to 900 steps)
- **Azimuth**: 0° to 360° (0 to 2000 steps)

### Speed Settings
- **SLEW_DELAY** (50) - Fast slewing between positions
- **TRACK_DELAY** (200) - Slow tracking speed
- **SLOW_DELAY** (100) - Careful approach to limits

## Memory Map

```
8000H - ALT_POS_LO      Current altitude position (16-bit)
8002H - AZ_POS_LO       Current azimuth position (16-bit)
8004H - ALT_TARGET_LO   Target altitude position (16-bit)
8006H - AZ_TARGET_LO    Target azimuth position (16-bit)
8008H - ALT_STEP_IDX    Current step pattern index for alt (0-3)
8009H - AZ_STEP_IDX     Current step pattern index for az (0-3)
800AH - MOTOR_STATE     Motor state flags
```

## Key Library Functions

### Initialization
```assembly
INIT_MOTORS             ; Initialize both motors and zero positions
```

### Low-Level Stepping
```assembly
STEP_ALT_CW             ; Step altitude motor clockwise (up)
STEP_ALT_CCW            ; Step altitude motor counter-clockwise (down)
STEP_AZ_CW              ; Step azimuth motor clockwise (east)
STEP_AZ_CCW             ; Step azimuth motor counter-clockwise (west)
```

### Multi-Step Movement
```assembly
MOVE_ALT                ; Move altitude by BC steps (signed)
MOVE_AZ                 ; Move azimuth by BC steps (signed)
```

### Position Control
```assembly
SET_TARGET_POSITION     ; Set target: DE=alt steps, HL=az steps
SLEW_TO_POSITION        ; Slew telescope to target position
HOME_TELESCOPE          ; Move to home position (0,0)
GET_CURRENT_POSITION    ; Get current position: DE=alt, HL=az
```

### Utility Functions
```assembly
DEGREES_TO_STEPS        ; Convert BC (degrees) to HL (steps)
DIV16                   ; 16-bit division: HL/BC -> HL quotient, DE remainder
DELAY                   ; General delay: BC = count
STEP_DELAY              ; Step delay: A = delay value
```

## User Interface

### TEC-1 Keypad Control

The system uses the TEC-1's 4×5 keypad (74C923) for all user input via NMI interrupts. Two input modes are supported:

**MODE 1: Precision (Deg/Min/Sec)**
```
AD → Toggle ALT/AZZ
+/- → Direction (required)
DD GO → Degrees
MM GO → Minutes
SS GO → Seconds → Executes
```

**MODE 2: Fast Step Count**
```
F → Enter fast mode
AD → Toggle ALT/AZZ
+/- → Direction (required)
SSSS GO → Step count (1-65535)
M GO → Speed multiplier (1-9x) → Executes
```

**Control Keys:**
- **AD** - Toggle between ALT (altitude) and AZZ (azimuth)
- **+/-** - Direction (mandatory, shows ERR if missing)
- **GO** - Confirm field / Double-press to execute immediately
- **C** - Skip current field (enters 0)
- **E** (RS) - Emergency stop, halt motors, preserve position
- **F** - Enter fast mode

**Example: Point to 45° 30' altitude:**
```
AD → +  → 45 GO → 30 GO → GO
```

See **KEYPAD-INTERFACE.md** for complete interface documentation.

## Usage Examples (Programmatic)

### Example 1: Initialize and Home
```assembly
START:
    LD      SP, 0FFFFH      ; Set stack pointer
    CALL    INIT_MOTORS     ; Initialize motors
    CALL    HOME_TELESCOPE  ; Move to home position (0,0)
```

### Example 2: Slew to a Position
```assembly
; Move to Alt=45°, Az=90°
; 45° = 250 steps, 90° = 500 steps

    LD      DE, 250         ; Altitude target
    LD      HL, 500         ; Azimuth target
    CALL    SET_TARGET_POSITION
    CALL    SLEW_TO_POSITION
```

### Example 3: Using Degrees
```assembly
; Convert 60 degrees to steps
    LD      BC, 60          ; 60 degrees
    CALL    DEGREES_TO_STEPS ; HL = steps (≈333)

    LD      D, 0
    LD      E, L            ; DE = altitude steps
    ; Now use DE for altitude target
```

### Example 4: Manual Stepping
```assembly
; Step altitude motor up 100 steps
    LD      BC, 100
STEP_LOOP:
    CALL    STEP_ALT_CW
    LD      A, SLEW_DELAY
    CALL    STEP_DELAY
    DEC     BC
    LD      A, B
    OR      C
    JR      NZ, STEP_LOOP
```

## Stepper Motor Drive Patterns

### 4-Step Sequence (Full Step)
```
Step 0: 0001b (Coil A)
Step 1: 0010b (Coil B)
Step 2: 0100b (Coil A')
Step 3: 1000b (Coil B')
```

Direction control:
- **Clockwise**: 0→1→2→3→0...
- **Counter-Clockwise**: 3→2→1→0→3...

### Half-Stepping (8-Step) - Optional
For smoother motion, uncomment half-step patterns in config.z80:
```
0001b, 0011b, 0010b, 0110b, 0100b, 1100b, 1000b, 1001b
```

## Coordinate System

```
Altitude (Alt):
  0° = Horizon
 45° = 45 degrees up
 90° = Zenith (straight up)

Azimuth (Az):
   0° = North
  90° = East
 180° = South
 270° = West
```

## Testing Routines

The main.z80 file includes several test routines:

```assembly
TEST_ALT_MOTOR          ; Test altitude motor full range (0-90°)
TEST_AZ_MOTOR           ; Test azimuth motor full rotation (360°)
TEST_BOTH_MOTORS        ; Test coordinated movement
CALIBRATE_HOME          ; Find home using limit switches (requires hardware)
```

## Building the Code

### Using z80asm (or similar assembler):
```bash
z80asm -o telescope.bin main.z80
```

### Using TASM (Table Assembler):
```bash
tasm -80 -b main.z80 telescope.bin
```

### For your TEC-1 or SBC:
Load the assembled binary to address 0000H and execute.

## Timing Considerations

### Step Rate
The delay values determine motor speed:
- **SLEW_DELAY = 50**: ~20ms per step = 50 steps/sec = 9°/sec
- **TRACK_DELAY = 200**: ~80ms per step = 12.5 steps/sec = 2.25°/sec

Adjust these values based on:
- Your Z80 clock frequency (typically 4 MHz)
- Motor torque requirements
- Load on the telescope
- Desired speed vs. accuracy

### Clock Frequency Scaling
If your Z80 runs at a different speed:
- **2 MHz**: Double all delay values
- **8 MHz**: Halve all delay values

## Advanced Features to Add

### 1. Acceleration/Deceleration
Add ramping for smoother motion:
```assembly
; Start slow, speed up, slow down near target
; Implement variable STEP_DELAY based on distance to target
```

### 2. Tracking
For sidereal tracking:
```assembly
; Move azimuth at constant rate: 15°/hour = 0.00417°/sec
; Call STEP_AZ_CW every ~4.8 seconds
```

### 3. Encoder Feedback
If you add encoders:
```assembly
; Read encoder positions and correct motor position
; Compare encoder count vs. commanded position
```

### 4. Limit Switches
Implement soft and hard limits:
```assembly
; Check STATUS_PORT for limit switch activation
; Prevent movement beyond physical limits
```

### 5. Serial Command Interface
Add UART communication:
```assembly
; Receive commands like "G045,090" = Go to Alt 45°, Az 90°
; Send position updates
```

## Troubleshooting

### Motors Not Moving
1. Check port addresses match your hardware
2. Verify power supply to motor drivers
3. Test with TEST_ALT_MOTOR or TEST_AZ_MOTOR
4. Check stepper coil connections

### Jerky Movement
1. Increase STEP_DELAY values
2. Check power supply stability
3. Verify motor current settings on driver
4. Consider half-stepping for smoother motion

### Position Drift
1. Missed steps due to too-fast movement
2. Increase STEP_DELAY
3. Reduce load on telescope
4. Add encoder feedback for verification

### Wrong Direction
1. Swap any two motor coil wires
2. Or modify step sequence (swap CW/CCW calls)

## Calibration Procedure

1. **Physical Setup**
   - Mount telescope to allow full range of motion
   - Install limit switches at home position
   - Verify motors can move freely

2. **Software Calibration**
   ```assembly
   CALL CALIBRATE_HOME    ; Find home position
   ```

3. **Verify Counts**
   - Manually position telescope to known angle
   - Compare encoder reading with expected value
   - Adjust STEPS_PER_10_DEG if needed

4. **Fine Tuning**
   - Test several known positions
   - Adjust gear ratio calculation if needed
   - Record actual vs. expected positions

## Connection Example (L298N Motor Driver)

```
Z80 Port Bit → L298N → Motor
─────────────────────────────
Alt Port:
  Bit 0 (A)  → IN1 → Coil A
  Bit 1 (B)  → IN2 → Coil B
  Bit 2 (A') → IN3 → Coil A'
  Bit 3 (B') → IN4 → Coil B'

Az Port:
  (Same as above for second motor)
```

## Future Enhancements

- [ ] PID control for smoother tracking
- [ ] Star catalog database integration
- [ ] Automatic alignment (plate solving)
- [ ] WiFi/Bluetooth control
- [ ] ASCOM/INDI driver compatibility
- [ ] Meridian flip handling
- [ ] Autoguiding support
- [ ] Weather sensor integration

## Version History

### Version 1.1 (2026-01-19) - Bug Fixes & Improvements ✅

**Critical bugs fixed:**
- ✅ SLEW_TO_POSITION early exit bug - both axes now complete properly
- ✅ Limit checking added - prevents hardware damage, enables wraparound
- ✅ Upgraded to full-step drive - 41% more torque

**New documentation:**
- ✅ USER-MANUAL.md - Complete step-by-step user guide
- ✅ STEPPING-OPTIONS.md - Motor drive mode comparison
- ✅ BUG-FIXES.md - Detailed bug fix documentation
- ✅ CHANGES-SUMMARY.md - Quick change reference
- ✅ DOCUMENTATION-INDEX.md - Documentation navigation

**See:** [CHANGES-SUMMARY.md](CHANGES-SUMMARY.md) for complete details

### Version 1.0 (Original)

**Features:**
- ✅ Dual-axis stepper motor control
- ✅ TEC-1 keypad interface (2 input modes)
- ✅ Position tracking with 16-bit counters
- ✅ Display multiplexing
- ✅ Sydney calibration data
- ✅ Sidereal tracking

**Known issues (fixed in v1.1):**
- ❌ SLEW would stop if one axis reached target first
- ❌ No limit checking (could damage hardware)
- ❌ Used wave drive (weak torque)

## License

This code is provided as-is for educational and hobbyist telescope automation projects.

## Author

Created for TEC-SCOPE project - DIY telescope automation using Z80 assembly.
