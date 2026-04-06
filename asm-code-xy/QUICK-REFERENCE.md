# Quick Reference Card

## Port Assignments
```
Port 03H  - Altitude Motor Control
Port 04H  - Azimuth Motor Control
Port 05H  - Status/Control (optional)

NOTE: TEC-1 SBC uses ports 00h-02h for keyboard and display
```

## Memory Map
```
8000H - ALT_POS_LO      Current altitude (low byte)
8001H - ALT_POS_HI      Current altitude (high byte)
8002H - AZ_POS_LO       Current azimuth (low byte)
8003H - AZ_POS_HI       Current azimuth (high byte)
8004H - ALT_TARGET_LO   Target altitude (low byte)
8005H - ALT_TARGET_HI   Target altitude (high byte)
8006H - AZ_TARGET_LO    Target azimuth (low byte)
8007H - AZ_TARGET_HI    Target azimuth (high byte)
```

## Step Patterns (4-step sequence)
```
Step 0: 0001b  (Coil A)
Step 1: 0010b  (Coil B)
Step 2: 0100b  (Coil A')
Step 3: 1000b  (Coil B')

Direction:
  CW:  0→1→2→3→0  (increment)
  CCW: 3→2→1→0→3  (decrement)
```

## Common Function Calls

### Initialization
```assembly
CALL INIT_MOTORS            ; Initialize and zero positions
```

### Low-Level Stepping
```assembly
CALL STEP_ALT_CW            ; Step altitude up one step
CALL STEP_ALT_CCW           ; Step altitude down one step
CALL STEP_AZ_CW             ; Step azimuth clockwise
CALL STEP_AZ_CCW            ; Step azimuth counter-clockwise
```

### Position Control
```assembly
; Set target position
LD   DE, 250                ; Alt = 250 steps (45°)
LD   HL, 500                ; Az = 500 steps (90°)
CALL SET_TARGET_POSITION

; Slew to target
CALL SLEW_TO_POSITION

; Go home
CALL HOME_TELESCOPE

; Get current position
CALL GET_CURRENT_POSITION   ; DE = alt, HL = az
```

### Movement
```assembly
; Move by relative steps
LD   BC, 100                ; 100 steps forward
CALL MOVE_ALT               ; Move altitude

LD   BC, -50                ; 50 steps backward
CALL MOVE_AZ                ; Move azimuth
```

## Conversion Formulas

### Degrees to Steps
```
steps = degrees × 5.555

For calculations:
  steps = (degrees × 56) / 10

Examples:
  45°  = 250 steps
  90°  = 500 steps
  180° = 1000 steps
  360° = 2000 steps
```

### Code Example:
```assembly
LD   BC, 45                 ; 45 degrees
CALL DEGREES_TO_STEPS       ; HL = 250 steps
```

### Steps to Degrees
```
degrees = steps × 0.18
        = steps / 5.555
```

## Typical Usage Pattern

### Complete Slew Example
```assembly
START:
    ; Initialize
    LD   SP, 0FFFFH
    CALL INIT_MOTORS

    ; Home telescope
    CALL HOME_TELESCOPE

    ; Set target: Alt=60°, Az=180°
    LD   DE, 333            ; 60° altitude
    LD   HL, 1000           ; 180° azimuth
    CALL SET_TARGET_POSITION

    ; Slew to target
    CALL SLEW_TO_POSITION

    ; Done
    HALT
```

### Manual Stepping Example
```assembly
    ; Step altitude up 50 steps
    LD   B, 50
STEP_LOOP:
    CALL STEP_ALT_CW
    LD   A, SLEW_DELAY
    CALL STEP_DELAY
    DJNZ STEP_LOOP
```

## Speed Control

### Delay Values (adjust for your clock speed)
```
SLEW_DELAY   = 50    Fast slewing (~20ms/step)
TRACK_DELAY  = 200   Slow tracking (~80ms/step)
SLOW_DELAY   = 100   Careful approach (~40ms/step)
```

### Custom Speed
```assembly
    CALL STEP_ALT_CW
    LD   A, 150         ; Custom delay value
    CALL STEP_DELAY
```

## Common Positions

### Celestial Coordinates (Examples)
```
Position          Alt      Az       Alt(steps)  Az(steps)
────────────────────────────────────────────────────────
Horizon/North     0°       0°       0           0
Horizon/East      0°       90°      0           500
Horizon/South     0°       180°     0           1000
Horizon/West      0°       270°     0           1500
Zenith            90°      -        900         -
Polaris (Sydney)  ~34°S    180°     ~189        1000
```

### Setting Example
```assembly
; Point to Polaris (from Sydney)
    LD   DE, 189            ; ~34° altitude
    LD   HL, 1000           ; 180° azimuth (south)
    CALL SET_TARGET_POSITION
    CALL SLEW_TO_POSITION
```

## Limits and Ranges

### Altitude (Vertical)
```
Minimum: 0°   (horizon)     = 0 steps
Maximum: 90°  (zenith)      = 900 steps
```

### Azimuth (Horizontal)
```
Minimum: 0°   (north)       = 0 steps
Maximum: 360° (full circle) = 2000 steps
```

### Check Limits
```assembly
; Check if target is in range
    LD   HL, (ALT_TARGET_LO)
    LD   DE, ALT_MAX
    OR   A
    SBC  HL, DE
    JP   C, TARGET_OK       ; If carry, target < max
    ; Handle error
TARGET_OK:
    ; Continue...
```

## Debugging Tips

### Visual Output Pattern
```assembly
; Output test pattern to see on LEDs
TEST_PATTERN:
    LD   A, 0001B
    OUT  (03H), A
    CALL DELAY
    LD   A, 0010B
    OUT  (03H), A
    CALL DELAY
    LD   A, 0100B
    OUT  (03H), A
    CALL DELAY
    LD   A, 1000B
    OUT  (03H), A
    CALL DELAY
    JR   TEST_PATTERN
```

### Position Display
```assembly
; Read and display current position
    LD   DE, (ALT_POS_LO)   ; Get altitude
    LD   HL, (AZ_POS_LO)    ; Get azimuth
    ; Output to display/serial
```

### Single Step Test
```assembly
; Step once and stop for observation
    CALL STEP_ALT_CW
    HALT                    ; Stop for inspection
```

## Error Handling

### Check Motor State
```assembly
    LD   A, (MOTOR_STATE)
    BIT  2, A               ; Check slew in progress
    JR   NZ, STILL_MOVING
    ; Motors stopped, safe to command
```

### Verify Position Match
```assembly
    LD   HL, (ALT_TARGET_LO)
    LD   DE, (ALT_POS_LO)
    OR   A
    SBC  HL, DE
    JR   Z, AT_TARGET
    ; Not at target
```

## Optimization Tips

### Faster Slewing
1. Reduce SLEW_DELAY value (carefully!)
2. Use half-stepping for smoother motion
3. Implement acceleration/deceleration

### Smoother Motion
1. Use smaller step delays
2. Implement ramping
3. Use microstepping (requires hardware)

### Power Saving
1. Disable motors when not moving
2. Reduce holding current
3. Use sleep modes

## Timing Reference

### At 4 MHz Z80 Clock
```
Instruction Cycles:
  NOP         4 cycles  = 1 µs
  LD A, n     7 cycles  = 1.75 µs
  OUT (n), A  11 cycles = 2.75 µs
  CALL addr   17 cycles = 4.25 µs
```

### Step Timing
```
With SLEW_DELAY = 50:
  Time per step ≈ 20 ms
  Steps per second ≈ 50
  Degrees per second ≈ 9°
  Full 360° rotation ≈ 40 seconds
```

## Cheat Sheet - Quick Copy/Paste

```assembly
; Initialize and home
CALL INIT_MOTORS
CALL HOME_TELESCOPE

; Slew to position
LD   DE, <alt_steps>
LD   HL, <az_steps>
CALL SET_TARGET_POSITION
CALL SLEW_TO_POSITION

; Move relative
LD   BC, <steps>
CALL MOVE_ALT           ; or MOVE_AZ

; Single step
CALL STEP_ALT_CW        ; or _CCW, _AZ_CW, _AZ_CCW

; Delay
LD   A, <delay_value>
CALL STEP_DELAY

; Get position
CALL GET_CURRENT_POSITION    ; Returns: DE=alt, HL=az
```

## Files Quick Index

```
config.z80          - Constants and configuration
stepper-lib.z80     - Motor control library
main.z80            - Main program with examples
simple-test.z80     - Simple test program
README.md           - Full documentation
HARDWARE.md         - Hardware connections
QUICK-REFERENCE.md  - This file
```

## Build Commands

```bash
# Using TASM
tasm -80 -b main.z80 telescope.bin
tasm -80 -b simple-test.z80 simple-test.bin

# Using z80asm
z80asm -o telescope.bin main.z80
z80asm -o simple-test.bin simple-test.z80

# Using make
make all            # Build everything
make main           # Build main program
make simple         # Build simple test
make clean          # Clean build files
```

## Testing Sequence

1. **Verify Hardware**: Check all connections with multimeter
2. **Simple Test**: Load simple-test.bin, verify single motor moves
3. **Library Test**: Load main.bin, run TEST_ALT_MOTOR
4. **Both Motors**: Run TEST_BOTH_MOTORS
5. **Position Test**: Try EXAMPLE_SLEW_1
6. **Production**: Integrate into your control system

---

**Remember**: Always test movements at slow speeds first, and ensure the telescope can move freely before powering on!
