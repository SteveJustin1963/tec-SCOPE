# Telescope Control System - Complete Overview

## System Architecture

This is a complete telescope control system for the **TEC-1 Z80 Single Board Computer**, designed for Sydney, Australia (34°S latitude). The system controls dual stepper motors for altitude and azimuth positioning with 10:1 gear reduction.

### Hardware Platform: TEC-1 SBC
- Z80 CPU @ 4 MHz
- 74C923 keyboard encoder (4×5 keypad, NMI on keypress)
- 6-digit 7-segment multiplexed display
- I/O ports 00h-02h reserved for keyboard/display
- Ports 03h-05h available for telescope motors

### Motor System
- 2× bipolar stepper motors (200 steps/rev, 1.8° per step)
- 10:1 gear reduction = 2000 steps per 360°
- Resolution: 0.18° per step (≈ 11 arc-minutes)
- L298N dual H-bridge motor drivers

## Software Modules

### 1. config.z80
**Purpose:** System configuration and constants

**Key Sections:**
- I/O port definitions (ALT_PORT=03h, AZ_PORT=04h, STATUS_PORT=05h)
- Motor specifications (steps, gear ratio, limits)
- Step patterns for 4-step sequence
- Memory locations (8000h-8015h: positions, targets, calibration)
- **Sydney calibration data:**
  - Latitude: 34°S
  - South Celestial Pole: Alt=34°, Az=180°
  - Magnetic declination: 12° East
  - Sidereal tracking rates
  - Reference stars (Canopus, Achernar, Sigma Octantis)

**Configuration Highlights:**
```assembly
LATITUDE        .EQU    34      ; Sydney latitude
SCP_ALTITUDE    .EQU    189     ; 34° in steps
SCP_AZIMUTH     .EQU    1000    ; South = 180°
MAG_DECLINATION .EQU    12      ; Magnetic correction
```

### 2. stepper-lib.z80
**Purpose:** Core motor control library

**Key Functions:**
- `INIT_MOTORS` - Initialize both motors, zero positions
- `STEP_ALT_CW/CCW` - Single-step altitude motor
- `STEP_AZ_CW/CCW` - Single-step azimuth motor
- `MOVE_ALT/MOVE_AZ` - Multi-step movement with signed offset
- `SLEW_TO_POSITION` - Coordinated dual-axis slew
- `SET_TARGET_POSITION` - Set target coordinates
- `HOME_TELESCOPE` - Return to (0,0)
- `GET_CURRENT_POSITION` - Query current position

**Features:**
- Automatic position tracking (16-bit counters)
- Signed movement (positive/negative)
- Coordinated dual-axis motion
- Configurable speeds (SLEW_DELAY, TRACK_DELAY)

### 3. keypad-input.z80
**Purpose:** TEC-1 keyboard interface and user input

**Key Components:**

**NMI Interrupt Handler (at 0066h):**
- Triggered by 74C923 on keypress
- Reads 5-bit key code from port 00h
- Dispatches to state machine

**Input Modes:**

**MODE 1: Precision (Deg/Min/Sec)**
- Enter coordinates as degrees, arc-minutes, arc-seconds
- Converts to steps using formula: `steps = arc_seconds × 5 / 9`
- Direct match to celestial coordinates

**MODE 2: Fast (Step Count + Speed)**
- Direct step count entry (1-65535)
- Variable speed multiplier (1-9x)
- Quick manual positioning

**Display Functions:**
- 7-segment text: "ALT", "AZZ", "PLUS", "NEG", "FAST"
- Numeric display: shows entry and countdown
- Uses TEC-1's non-sequential segment mapping

**State Machine:**
```
IDLE → AD/F key → MODE SELECTED
  ↓
WAIT_SIGN → +/- key → DIRECTION SET
  ↓
ENTER_FIELDS → Digits/GO/C → FIELD ENTRY
  ↓
EXECUTE → Motor movement → IDLE
```

**Control Keys:**
- **AD** - Toggle ALT/AZZ
- **F** - Fast mode
- **+/-** - Direction (mandatory)
- **GO** - Confirm / Double-GO to execute
- **C** - Skip field
- **E (RS)** - Emergency stop (preserves position)

### 4. main.z80
**Purpose:** Main program and integration

**Startup Sequence:**
1. Initialize stack (SP = 0FFFFh)
2. Initialize motors (INIT_MOTORS)
3. Initialize keypad system (INIT_INPUT_SYSTEM)
4. Enable interrupts (EI)
5. Enter main loop (HALT waiting for NMI)

**Main Loop:**
- Idle loop (HALT instruction)
- All work done in NMI handler
- Motors execute movements outside interrupt
- Returns to idle after completion

**Calibration Routines:**
- `CALIBRATE_POLAR_ALIGNMENT` - Complete polar setup for Sydney
- `ALIGN_TO_SOUTH_HORIZON` - Point south using compass
- `ALIGN_TO_SCP` - Tilt to celestial pole (34°)
- `ALIGN_TO_CANOPUS` - Use bright star reference
- `START_SIDEREAL_TRACKING` - Enable tracking mode
- `STORE_TRUE_SOUTH` - Save compass alignment offset

**Demo Programs:**
- `EXAMPLE_HOME` - Return to home position
- `EXAMPLE_SLEW_1` - Move to 45°, 90°
- `EXAMPLE_SLEW_2` - Move to 60°, 180°
- Test routines for individual motors

## Memory Map

### Motor Control (8000h-800Ah)
```
8000h - ALT_POS_LO      Current altitude position
8001h - ALT_POS_HI
8002h - AZ_POS_LO       Current azimuth position
8003h - AZ_POS_HI
8004h - ALT_TARGET_LO   Target altitude
8005h - ALT_TARGET_HI
8006h - AZ_TARGET_LO    Target azimuth
8007h - AZ_TARGET_HI
8008h - ALT_STEP_IDX    Current step index (0-3)
8009h - AZ_STEP_IDX
800Ah - MOTOR_STATE     Motor status flags
```

### Calibration Data (800Bh-8015h)
```
800Bh - CAL_OFFSET_ALT  Altitude alignment offset
800Dh - CAL_OFFSET_AZ   Azimuth alignment offset
800Fh - CAL_REF_STAR_ALT Reference star altitude
8011h - CAL_REF_STAR_AZ  Reference star azimuth
8013h - CAL_TRUE_SOUTH  True south azimuth reading
8015h - CAL_STATUS      Calibration status flags
```

### Keypad Input State (8020h-802Fh)
```
8020h - INPUT_MODE      0=idle, 1=AD mode, 2=F mode
8021h - INPUT_AXIS      0=ALT, 1=AZZ
8022h - INPUT_SIGN      0=positive, 1=negative
8023h - INPUT_STATE     Field state (0-5)
8024h - INPUT_DEG       Degrees or step count
8026h - INPUT_MIN       Minutes
8028h - INPUT_SEC       Seconds
802Ah - INPUT_SPEED     Speed multiplier (1-9)
802Bh - INPUT_BUFFER    Temporary accumulator
802Dh - INPUT_GO_COUNT  GO press counter
802Eh - LAST_KEY        Last key code
802Fh - TEMP_SPEED      Temporary speed storage
```

## I/O Port Map

### TEC-1 Reserved Ports
```
00h - IN  - Keyboard data (74C923, 5-bit key code)
01h - OUT - Display digit select (6 bits, one per digit)
02h - OUT - Display segment data (8 bits: a,f,g,b,dp,c,e,d)
```

### Telescope Motor Ports
```
03h - OUT - Altitude motor (4-bit step pattern)
04h - OUT - Azimuth motor (4-bit step pattern)
05h - IN  - Status/limit switches (optional)
```

**Step Pattern (4 bits):**
```
Bit 0: Coil A
Bit 1: Coil B
Bit 2: Coil A' (reverse)
Bit 3: Coil B' (reverse)

Patterns: 0001, 0010, 0100, 1000 (rotate for steps)
```

## Data Flow

### User Input Flow
```
Keypress → NMI interrupt → Read port 00h
  ↓
State machine processing → Update INPUT_* variables
  ↓
Display feedback → Update 7-segment via ports 01h/02h
  ↓
GO GO → Execute movement
  ↓
Convert to steps → Apply direction → Move motors
  ↓
Output to port 03h/04h → Motor drivers → Physical movement
  ↓
Update position counters → Return to idle
```

### Position Tracking Flow
```
STEP_ALT_CW → Increment ALT_STEP_IDX (0-3)
  ↓
Lookup pattern from STEP_TABLE
  ↓
OUT (ALT_PORT), pattern
  ↓
Increment ALT_POS_LO/HI
  ↓
Position preserved in memory
```

## Calibration Procedure (Sydney, Australia)

### 1. Initial Setup
- Mount telescope on wedge at 34° tilt (latitude angle)
- Point mount south using compass
- Account for 12° magnetic declination (point to 168° magnetic)

### 2. Software Calibration
```assembly
CALL INIT_MOTORS              ; Zero positions
CALL CALIBRATE_POLAR_ALIGNMENT ; Polar alignment wizard
  → ALIGN_TO_SOUTH_HORIZON    ; Point south at horizon
  → ALIGN_TO_SCP              ; Tilt to 34°
  → STORE_TRUE_SOUTH          ; Save compass offset
```

### 3. Star Alignment
```assembly
CALL ALIGN_TO_CANOPUS         ; Use bright southern star
CALL STORE_REFERENCE_STAR     ; Save as reference
```

### 4. Enable Tracking
```assembly
CALL START_SIDEREAL_TRACKING  ; Enable westward tracking
```

**Sidereal Rate:** 1 step every ~44 seconds (westward for southern hemisphere)

## Usage Examples

### Example 1: Point to Canopus
**Using Keypad (MODE 1):**
```
AD → "ALT   " (select altitude)
+ → "PLUS  "
78 GO → "    78" (degrees at culmination)
GO → Execute (0' 0")

AD → "AZZ   " (toggle to azimuth)
+ → "PLUS  "
180 GO → "   180" (south)
GO → Execute
```

### Example 2: Quick Manual Adjustment
**Using Keypad (MODE 2 - Fast):**
```
F → "FAST  "
AD → "ALT   "
+ → "PLUS  "
500 GO → "   500" (500 steps)
3 GO → Execute at 3x speed
```

### Example 3: Programmatic Slew
```assembly
; Slew to 60°, 180°
LD   DE, 333         ; 60° altitude = 333 steps
LD   HL, 1000        ; 180° azimuth = 1000 steps
CALL SET_TARGET_POSITION
CALL SLEW_TO_POSITION
```

### Example 4: Emergency Stop
**Press E key (RS):**
- Motors halt immediately
- Outputs 0000b to both ports
- Position preserved in ALT_POS/AZ_POS
- Ready for new command

## Build and Deploy

### Build Commands
```bash
# Using make (recommended)
make all            # Build main + simple test

# Using TASM directly
tasm -80 -b main.z80 telescope.bin

# Using z80asm
z80asm -o telescope.bin main.z80
```

### Binary Output
- **telescope.bin** - Main program with keypad interface
- **simple-test.bin** - Standalone motor test

### Loading to TEC-1
1. Power on TEC-1
2. Load telescope.bin to address 0000h
3. Execute from 0000h
4. System initializes and waits for keypad input

## Testing Procedure

### 1. Initial Power-On Test
```assembly
; Load simple-test.bin
; Runs automated motor tests:
; - Alt CW 100 steps
; - Alt CCW 100 steps
; - Az CW 100 steps
; - Both motors together
```

### 2. Keypad Interface Test
```
Press AD → Should display "ALT   "
Press AD → Should toggle to "AZZ   "
Press + → Should display "PLUS  "
```

### 3. Single Axis Movement
```
AD → + → 10 GO GO
Should move 10° altitude upward
Display counts down steps
```

### 4. Calibration Verification
```assembly
CALL ALIGN_TO_SCP     ; Point to south celestial pole
; Manually verify pointing at 34° altitude, 180° azimuth
```

## Troubleshooting

### Issue: Display Shows "ERR  "
**Cause:** Missing +/- sign after AD/AZZ selection
**Fix:** Always press + or - before entering numbers

### Issue: Motors Don't Move
**Check:**
1. Power to L298N motor drivers
2. Motor connections (4 wires per motor)
3. Port outputs (use oscilloscope on 03h/04h)
4. Position limits (ALT: 0-900, AZ: 0-2000)

### Issue: Wrong Direction
**Fix:**
- Swap any two motor wires (reverses one coil)
- Or modify software: swap CW/CCW function calls

### Issue: Position Drift
**Causes:**
- Steps too fast (increase SLEW_DELAY)
- Load too heavy (reduce weight or use slower speed)
- Mechanical binding (check mount movement)

### Issue: Display Garbled
**Fix:**
- Press RS (E key) to reset
- May need hardware reset if unresponsive

## Performance Specifications

### Speed
- **Normal slew:** 50 steps/sec (SLEW_DELAY=50) = 9°/sec
- **Fast mode 9x:** 450 steps/sec = 81°/sec
- **Full rotation:** 40 seconds (normal speed)

### Accuracy
- **Resolution:** 0.18° per step = 10.8 arc-minutes
- **Repeatability:** ±1 step (±0.18°) with proper calibration
- **Tracking drift:** < 0.5° per minute (after calibration)

### Limits
- **Altitude:** 0° to 90° (horizon to zenith)
- **Azimuth:** 0° to 360° (full rotation)
- **Step count:** 16-bit (0-65535 steps)
- **Speed multiplier:** 1-9x

## Future Enhancements

**Planned:**
- [ ] Star catalog with named objects
- [ ] Automatic position display in main loop
- [ ] Save/recall favorite positions to EEPROM
- [ ] Goto mode with automatic star lookup
- [ ] Encoder feedback for closed-loop control
- [ ] Serial interface for PC control
- [ ] ASCOM driver compatibility

**Advanced:**
- [ ] Autoguiding via CCD camera input
- [ ] PEC (Periodic Error Correction)
- [ ] GoTo++ with plate solving
- [ ] WiFi control via ESP8266 module
- [ ] Weather sensor integration

## References

**Documentation:**
- KEYPAD-INTERFACE.md - Complete keypad guide
- HARDWARE.md - Wiring and connections
- QUICK-REFERENCE.md - Quick reference card
- flow.md - Program flowcharts
- tec1-sbc-spec.md - TEC-1 hardware details

**External:**
- Z80 CPU User Manual
- L298N Datasheet
- 74C923 Keyboard Encoder Datasheet
- TASM Assembler Documentation

## License

This code is provided as-is for educational and hobbyist telescope automation projects.

---

**Created for the TEC-SCOPE project**
Sydney, Australia - Southern Hemisphere Astronomy
Z80 Assembly Programming - Vintage Computer Telescope Control
