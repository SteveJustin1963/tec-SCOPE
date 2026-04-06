# Motor Control Z80 Code Usage Guide

## Overview
Compact Z80 assembly code for controlling 2 DC motors with PWM and reading 2 encoders via SPI.
- **Size**: ~700 bytes (well under 2K limit)
- **ORG**: 0800h
- **RAM used**: 0900h-0914h (20 bytes)

## Port Assignments

| Port | Purpose | Bits |
|------|---------|------|
| 03h | SPI Control | bit0=CLK, bit1=MOSI, bit2=CS1, bit3=CS2, bit4=MISO(in) |
| 04h | Motor Direction | bit0=M1_DIR, bit1=M2_DIR |
| 05h | Motor 1 PWM | 0-255 (0=off, 255=full speed) |
| 06h | Motor 2 PWM | 0-255 (0=off, 255=full speed) |
| 07h | Input | Joystick/buttons for manual control |

## Hardware Connections

### SPI Bus (Port 03h)
```
Z80 Port 03h    ATtiny85 (Both)
bit 0 (CLK)  -> PB3 (SCK)
bit 1 (MOSI) -> (not used, ATtiny is slave)
bit 2 (CS1)  -> PB2 on U2 (Alt encoder)
bit 3 (CS2)  -> PB2 on U5 (Az encoder)
bit 4 (MISO) <- PB4 from both ATtiny85s (shared)
```

### Motor 1 (Altitude)
```
Z80 Port 04h bit 0 -> L298N IN1 (direction)
Z80 Port 05h       -> L298N ENA (PWM via transistor)
L298N OUT1/OUT2    -> DC Motor 1
```

### Motor 2 (Azimuth)
```
Z80 Port 04h bit 1 -> L298N IN3 (direction)
Z80 Port 06h       -> L298N ENB (PWM via transistor)
L298N OUT3/OUT4    -> DC Motor 2
```

### Input Port (Port 07h)
```
Joystick/Buttons to Port 07h:
bit 0: Motor 1 Forward button
bit 1: Motor 1 Reverse button
bit 2: Motor 2 Forward button
bit 3: Motor 2 Reverse button
```

## Memory Map

```
0800h - 0BFFh: Program code (~700 bytes used)
0900h - 0914h: Variables (20 bytes)
  0900h: MOTOR1_PWM (1 byte)
  0901h: MOTOR2_PWM (1 byte)
  0902h: MOTOR1_DIR (1 byte)
  0903h: MOTOR2_DIR (1 byte)
  0904h: ENC1_POS (4 bytes, 32-bit encoder position)
  0908h: ENC2_POS (4 bytes, 32-bit encoder position)
  090Ch: TARGET1_POS (4 bytes, 32-bit target)
  0910h: TARGET2_POS (4 bytes, 32-bit target)
  0914h: PWM_COUNTER (1 byte)
```

## Usage Examples

### Example 1: Basic Startup
```assembly
; Load and run the code
ORG 0800h
; Code is assembled starting at 0800h
; Jump to START to begin
JP 0800h
```

### Example 2: Set Target Position (GoTo mode)
```assembly
; Set Motor 1 (Alt) target to position 1000
LD BC, 0000h
LD DE, 03E8h    ; 0x000003E8 = 1000 decimal
CALL SET_TARGET1

; Set Motor 2 (Az) target to position 5000
LD BC, 0000h
LD DE, 1388h    ; 0x00001388 = 5000 decimal
CALL SET_TARGET2

; Motors will now automatically move to these positions
; The main loop handles it
```

### Example 3: Manual Control Mode
```assembly
; Replace the main loop to use manual control
START:
        CALL INIT_SYSTEM

MANUAL_LOOP:
        CALL READ_ENCODERS      ; Still read encoders for display
        CALL MANUAL_CONTROL     ; Use joystick instead of auto
        CALL PWM_CYCLE          ; Generate PWM
        JP MANUAL_LOOP
```

### Example 4: Read Current Position
```assembly
; Read Motor 1 position (32-bit value at ENC1_POS)
LD HL, ENC1_POS
LD B, (HL)      ; MSB
INC HL
LD C, (HL)
INC HL
LD D, (HL)
INC HL
LD E, (HL)      ; LSB
; BCDE now contains 32-bit encoder count

; Convert to degrees (simplified):
; degrees = (count * 360) / (600 * 10)
; degrees = count * 360 / 6000
; degrees = count * 0.06
```

## Control Modes

### Mode 1: Automatic Position Control (Default)
The main loop compares encoder position to target position and adjusts PWM automatically.

**Proportional Control:**
- Error = Target - Current
- PWM = Error × 4 (gain factor)
- Direction based on error sign
- Max PWM limited to 200 (50 × 4)

### Mode 2: Manual Joystick Control
Call `MANUAL_CONTROL` instead of `UPDATE_MOTORS` in main loop.
- Joystick input on port 07h controls motors directly
- PWM fixed at 80 (~31% duty cycle for slow manual movement)

## PWM Details

**Software PWM:**
- 8-bit resolution (0-255)
- Counter increments each cycle
- Motor ON when duty > counter
- Frequency: ~1kHz at 4MHz Z80 clock

**Hardware PWM (alternative):**
If your L298N has hardware PWM input, you can:
1. Output duty cycle value to ports 05h/06h
2. Use external hardware PWM generator
3. Remove PWM_CYCLE routine to save space

## Calibration

### Step 1: Verify Encoder Direction
```assembly
; Move motor 1 forward
LD A, 100
LD (MOTOR1_PWM), A
LD A, 1
LD (MOTOR1_DIR), A

; Wait and read encoder
CALL READ_ENCODERS
; Check if ENC1_POS increases
; If decreases, swap encoder A/B wires or invert direction bit
```

### Step 2: Measure Counts Per Revolution
```assembly
; Start at known position
CALL READ_ENCODERS
LD HL, ENC1_POS+3
LD A, (HL)
LD B, A         ; Save starting count LSB

; Manually rotate telescope exactly 360 degrees
; Read again
CALL READ_ENCODERS
LD HL, ENC1_POS+3
LD A, (HL)
SUB B           ; Difference = counts per 360°
; Should be ~6000 for 600 PPR encoder with 10:1 gearing
```

### Step 3: Set Proportional Gain
Adjust the gain in UPDATE_MOTORS:
```assembly
; Current code uses x4 gain (two SLA instructions)
SLA A           ; x2
SLA A           ; x4

; For slower, smoother movement use x2:
SLA A           ; x2 only

; For faster response use x8:
SLA A           ; x2
SLA A           ; x4
SLA A           ; x8
```

## Optimization Tips

### Save Space:
- Remove MANUAL_CONTROL if not needed (~60 bytes)
- Use 16-bit positions instead of 32-bit (~40 bytes saved)
- Combine SET_TARGET1/SET_TARGET2 into one routine

### Improve Performance:
- Use interrupt-driven PWM instead of polling
- Implement PID control (add integral and derivative terms)
- Add acceleration/deceleration ramps for smooth starts/stops

## Troubleshooting

| Problem | Cause | Solution |
|---------|-------|----------|
| Motors don't move | PWM = 0 or bad wiring | Check MOTOR_PWM variables, verify L298N connections |
| Wrong direction | Direction bit inverted | Swap L298N IN1/IN2 or IN3/IN4 wires |
| Encoders don't count | SPI not working | Use oscilloscope on port 03h to verify CLK/CS signals |
| Encoders count wrong direction | A/B swapped | Swap encoder A/B wires on ATtiny85 |
| Jerky movement | PWM frequency too low | Reduce PWM_DELAY value |
| Won't reach target | Gain too low | Increase SLA instructions in UPDATE_MOTORS |
| Oscillates at target | Gain too high | Decrease gain or add deadband |

## Next Steps

1. **Assemble the code** using Z80 assembler (e.g., z80asm, pasmo)
2. **Load to TEC-1** at address 0800h
3. **Test SPI** - Call READ_ENCODERS, verify data changes when you rotate motors
4. **Test PWM** - Set MOTOR1_PWM to 100, verify motor spins
5. **Test direction** - Toggle MOTOR1_DIR, verify direction changes
6. **Test closed-loop** - Set targets, watch motors move to position
7. **Add display** - Output encoder positions to 7-segment display
8. **Add Stellarium** - Interface via serial port for planetarium control
