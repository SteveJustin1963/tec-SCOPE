# Hardware Connection Guide

## Overview

This document describes the hardware connections for the telescope stepper motor control system using a Z80 SBC (Single Board Computer).

This project is specifically designed for the **TEC-1** Z80 trainer/SBC, but can be adapted to any Z80 system with available I/O ports.

## System Diagram

```
┌──────────────────────────────────────────────────────────────────┐
│                         Z80 SBC                                  │
│                                                                  │
│  ┌────────┐                                                     │
│  │  Z80   │                                                     │
│  │  CPU   │                                                     │
│  └────┬───┘                                                     │
│       │                                                          │
│  ┌────┴──────┐                                                  │
│  │ I/O Ports │                                                  │
│  └─┬─────┬───┘                                                  │
│    │     │                                                      │
└────┼─────┼──────────────────────────────────────────────────────┘
     │     │
     │     └──────────────────┐
     │                        │
  Port 03H                 Port 04H
  (Alt Motor)              (Az Motor)
     │                        │
     ▼                        ▼
┌─────────┐              ┌─────────┐
│ L298N   │              │ L298N   │
│ Motor   │              │ Motor   │
│ Driver  │              │ Driver  │
└────┬────┘              └────┬────┘
     │                        │
     ▼                        ▼
┌─────────┐              ┌─────────┐
│ Stepper │              │ Stepper │
│ Motor   │              │ Motor   │
│ (Alt)   │              │ (Az)    │
└─────────┘              └─────────┘
     │                        │
     │    ┌──────────────┐    │
     └────┤  10:1 Gear   │────┘
          │  Reduction   │
          └──────┬───────┘
                 │
          ┌──────┴───────┐
          │  Telescope   │
          │   Mount      │
          └──────────────┘
```

## TEC-1 SBC Integration

### Port Allocation

The TEC-1 uses ports 00h-02h for its built-in peripherals:

| Port | Direction | TEC-1 Function | Available? |
|------|-----------|----------------|------------|
| 00h  | Input     | Keyboard data (74C923, triggers NMI) | **NO - Reserved** |
| 01h  | Output    | Display digit select (multiplexes common ground) | **NO - Reserved** |
| 02h  | Output    | Display segment data (drives a-g + dp) | **NO - Reserved** |
| 03h  | Available | **Altitude motor** | **YES - Used by this project** |
| 04h  | Available | **Azimuth motor** | **YES - Used by this project** |
| 05h  | Available | **Status port** (optional) | **YES - Used by this project** |
| 06h  | Available | Expansion | YES |
| 07h  | Available | Expansion | YES |

**Important:** The TEC-1 uses partial I/O address decoding, so ports mirror every 8 addresses. Port 03h also responds at 0Bh, 13h, 1Bh, etc. This is normal and acceptable for this project.

### TEC-1 Hardware Connections

The TEC-1 expands via its **edge connector**. You'll need to access the following signals:

**Data Bus (D0-D7):**
- Connect to 74LS244 buffer inputs for port 03h and 04h
- Or connect directly to L298N inputs if using port decoder

**Control Signals:**
- **IORQ̅** (I/O Request) - Active low during I/O operations
- **WR̅** (Write) - Active low during output operations
- **A0-A7** (Address bus) - For port decoding

**Power:**
- **+5V** - Logic power for buffers and L298N logic
- **GND** - Common ground

### Port Decoding for TEC-1

You have two options for adding ports 04h and 05h:

#### Option 1: Use Existing Decoder (Simple)

Since the TEC-1's 74LS138 decoder already provides outputs, you can use the available outputs:
- Port 03h = Y3 output
- Port 04h = Y4 output

Check if Y3/Y4 are already wired on your TEC-1 variant, or add latches to these decoder outputs.

#### Option 2: Add External Decoder (Recommended)

Add a second 74LS138 decoder for clean port separation:

```
TEC-1 Edge Connector
    │
    ├─── A0, A1, A2 ──→ 74LS138 A, B, C inputs
    ├─── IORQ̅ ────────→ 74LS138 G1̅ (enable)
    ├─── WR̅ ──────────→ 74LS138 G2A̅ (enable)
    └─── A2 inverted ─→ 74LS138 G2B (select ports 00-07)

74LS138 Outputs:
    Y0 ─────→ (00h - keyboard data, 74C923)
    Y1 ─────→ (01h - display digit select)
    Y2 ─────→ (02h - display segment data)
    Y3 ─────→ Port 03h latch enable (Altitude motor)
    Y4 ─────→ Port 04h latch enable (Azimuth motor)
    Y5 ─────→ Port 05h latch enable (Status)
    Y6 ─────→ (06h - expansion)
    Y7 ─────→ (07h - expansion)
```

### TEC-1 Display Integration (Optional)

You can show motor position on the TEC-1's 6-digit display by adding display update code:

```assembly
; Update TEC-1 display with current position
UPDATE_DISPLAY:
    LD   HL, (ALT_POS_LO)    ; Get altitude position
    ; Convert to decimal/hex
    ; Write to display using ports 01h (digit) and 02h (segments)
    RET
```

See the TEC-1 specification for display multiplexing routines.

## Component List

### Required Components

1. **Z80 Single Board Computer**
   - TEC-1, SC-1, or similar Z80 SBC
   - 4 MHz clock (typical)
   - I/O ports available

2. **Stepper Motors** (x2)
   - Type: Bipolar stepper
   - Steps: 200 per revolution (1.8° per step)
   - Voltage: 12V typical
   - Current: 1-2A per phase
   - Recommended: NEMA 17 or NEMA 23

3. **Motor Drivers** (x2)
   - Option 1: L298N Dual H-Bridge (recommended for testing)
   - Option 2: A4988 Stepper Driver
   - Option 3: DRV8825 Stepper Driver
   - Option 4: TB6600 Stepper Driver (for larger motors)

4. **Power Supply**
   - 12V DC, 3-5A (depends on motors)
   - Separate from Z80 logic power
   - Common ground with Z80

5. **Gearing**
   - 10:1 reduction gearbox for each axis
   - Or worm drive mechanism
   - Mounted on telescope

6. **Miscellaneous**
   - Hook-up wire (22-26 AWG)
   - Breadboard or PCB
   - 74LS244 or 74HCT244 buffers (for I/O port expansion)
   - Pull-up/pull-down resistors (10kΩ)
   - Bypass capacitors (0.1µF)

## Understanding the L298N Motor Driver

### What the L298N Does

The **L298N is a dual H-bridge motor driver IC** that acts as the critical interface between the low-power Z80 digital signals and the high-power stepper motor coils.

#### 1. Power Amplification

The Z80's I/O ports output low-current digital signals (typically 5V at a few milliamps). Stepper motors require much more power - typically 12V at 1-2 amperes per coil. The Z80 cannot directly drive these motors.

The L298N solves this by:
- Accepting low-current control signals from the Z80 (IN1-IN4 pins)
- Switching high-current outputs to the motor coils (OUT1-OUT4 pins)
- Acting as a power amplifier/buffer between logic and motor

#### 2. H-Bridge Operation

An H-bridge is a circuit that allows current to flow in either direction through a load (motor coil). The L298N contains **two** H-bridges, allowing it to control one bipolar stepper motor.

Each H-bridge pair (controlled by IN1/IN2 or IN3/IN4) drives one motor coil and can:
- Energize the coil in forward direction (e.g., IN1=HIGH, IN2=LOW)
- Energize the coil in reverse direction (e.g., IN1=LOW, IN2=HIGH)
- Turn off the coil completely (both LOW)

#### 3. Step Pattern Translation

The Z80 outputs 4-bit patterns corresponding to the stepping sequence:

| Z80 Output | Active Input | Coils Energized | Motor Action |
|------------|--------------|-----------------|--------------|
| 0001b | IN1 | Coil A forward | Step position 0 |
| 0010b | IN3 | Coil B forward | Step position 1 |
| 0100b | IN2 | Coil A reverse | Step position 2 |
| 1000b | IN4 | Coil B reverse | Step position 3 |

By cycling through these patterns (either incrementing or decrementing), the motor rotates in the desired direction.

#### 4. Dual Motor Configuration

The L298N is called "dual" because it has two independent H-bridges. In this project:
- **Each L298N module** controls **one bipolar stepper motor** (using both H-bridges)
- **Two L298N modules** are needed - one for altitude, one for azimuth
- Each motor uses all 4 control inputs (IN1-IN4)

#### 5. Protection and Isolation

The L298N provides important protection:
- **Electrical isolation** between Z80 logic power (+5V) and motor power (+12V)
- **Back-EMF protection** - when motor coils switch off, they generate voltage spikes that could damage the Z80
- **Current limiting** - internal protection against overcurrent
- **Separate grounds** - though grounds are connected, the power supplies are separate

#### 6. Enable Pins (ENA, ENB)

The L298N has enable pins that control whether the outputs are active:
- **ENA** enables H-bridge A (OUT1/OUT2)
- **ENB** enables H-bridge B (OUT3/OUT4)
- Typically tied to +5V for always-on operation
- Can be connected to PWM for current limiting or motor disable

### Why You Need It

**Without the L298N:**
- Z80 outputs: 5V @ ~5mA = 25mW
- Stepper motor needs: 12V @ 1.5A = 18W per coil
- The Z80 would immediately burn out trying to drive the motor directly

**With the L298N:**
- Z80 → L298N control inputs (low power)
- L298N → Motor coils (high power from separate 12V supply)
- Safe operation with proper power delivery

## Detailed Connections

### Z80 I/O Port to L298N Driver

#### Altitude Motor (Port 03H)

```
Z80 Port 03H          L298N Driver         Stepper Motor
─────────────────────────────────────────────────────────
Bit 0 (D0)  ────→ IN1
                      │                     Coil A+
                      ├───────→ OUT1 ────→ (Red)
                      │         OUT2 ────→ (Green)
Bit 2 (D2)  ────→ IN2 │                     Coil A-
                      │
Bit 1 (D1)  ────→ IN3 │
                      │                     Coil B+
                      ├───────→ OUT3 ────→ (Blue)
                      │         OUT4 ────→ (Yellow)
Bit 3 (D3)  ────→ IN4 │                     Coil B-

                  +12V ←──── Power Supply
                  GND  ←──── Common Ground
                  ENA  ←──── +5V (enable)
```

#### Azimuth Motor (Port 04H)

Same connections as altitude motor, connected to port 04H.

### Pin Assignments

#### Z80 SBC to Buffer (74LS244)

```
Z80 Data Bus    74LS244 Buffer    To Motor Driver
────────────────────────────────────────────────
D0  ──────────→ 1A1 ────→ 1Y1 ────→ IN1 (Alt)
D1  ──────────→ 1A2 ────→ 1Y2 ────→ IN2 (Alt)
D2  ──────────→ 1A3 ────→ 1Y3 ────→ IN3 (Alt)
D3  ──────────→ 1A4 ────→ 1Y4 ────→ IN4 (Alt)

D0  ──────────→ 2A1 ────→ 2Y1 ────→ IN1 (Az)
D1  ──────────→ 2A2 ────→ 2Y2 ────→ IN2 (Az)
D2  ──────────→ 2A3 ────→ 2Y3 ────→ IN3 (Az)
D3  ──────────→ 2A4 ────→ 2Y4 ────→ IN4 (Az)
```

### Stepper Motor Wiring

#### Typical Bipolar Stepper Color Code

```
Wire Color    Function       L298N Output
─────────────────────────────────────────
Red           Coil A+        OUT1
Green         Coil A-        OUT2
Blue          Coil B+        OUT3
Yellow        Coil B-        OUT4
```

**Note:** Wire colors may vary by manufacturer. Use a multimeter to identify coil pairs:
- Measure resistance between wires
- Wires of the same coil will show ~1-10Ω resistance
- Wires of different coils will show infinite resistance

## L298N Module Connections

### Typical L298N Module Pinout

```
┌─────────────────────────────────┐
│        L298N Motor Driver        │
├─────────────────────────────────┤
│                                 │
│  IN1  ○────  Control inputs     │ ← From Z80 Port Bit 0
│  IN2  ○────  from Z80           │ ← From Z80 Port Bit 1
│  IN3  ○────                     │ ← From Z80 Port Bit 2
│  IN4  ○────                     │ ← From Z80 Port Bit 3
│                                 │
│  ENA  ○────  Enable A (tie +5V) │ ← +5V or PWM
│  ENB  ○────  Enable B (tie +5V) │ ← +5V or PWM
│                                 │
│  +12V ○────  Motor power        │ ← 12V from power supply
│  GND  ○────  Ground             │ ← Common ground
│  +5V  ○────  Logic power        │ ← From Z80 +5V
│                                 │
│  OUT1 ○────┐                    │
│  OUT2 ○────┼─ To stepper motor  │
│  OUT3 ○────┤                    │
│  OUT4 ○────┘                    │
│                                 │
└─────────────────────────────────┘
```

### Connection Notes

1. **Enable Pins (ENA, ENB)**
   - Tie to +5V for always-on
   - Or connect to PWM for current limiting
   - Some modules have jumpers - keep them installed

2. **Power Supply**
   - Use separate 12V supply for motors
   - Z80 and logic use +5V supply
   - **CRITICAL**: Connect grounds together (common ground)

3. **Heat Sinking**
   - L298N can get hot
   - Ensure good ventilation
   - Add heat sink if running continuously

## I/O Port Decoding

### TEC-1 Port Decoding with 74LS138

The TEC-1 uses a 74LS138 3-to-8 decoder for I/O port selection:

```
74LS138 Decoder (on TEC-1)
────────────────
A0 ───→ A
A1 ───→ B
A2 ───→ C

/IORQ ─→ /G1 (enable during I/O)
WR̅    ─→ /G2A (enable during write)
+5V ──→ G2B (always enabled)

Outputs:
/Y0 ────→ Port 00H (Keyboard data - TEC-1)
/Y1 ────→ Port 01H (Display digit select - TEC-1)
/Y2 ────→ Port 02H (Display segment data - TEC-1)
/Y3 ────→ Port 03H (Alt Motor) ← **Available**
/Y4 ────→ Port 04H (Az Motor)  ← **Available**
/Y5 ────→ Port 05H (Status)    ← **Available**
/Y6 ────→ Port 06H (Expansion) ← **Available**
/Y7 ────→ Port 07H (Expansion) ← **Available**
```

**Note:** On the TEC-1, Y0-Y2 are already used for keyboard and display. You'll need to access Y3 and Y4 outputs for the motor drivers, either from the existing decoder or by adding latches (74LS273) controlled by these outputs.

## Power Supply Design

### Recommended Configuration

```
┌────────────┐
│ 12V 5A     │
│ Power      │
│ Supply     │
└─────┬──────┘
      │
      ├──────────────→ L298N +12V (Alt)
      ├──────────────→ L298N +12V (Az)
      │
      │   ┌────────┐
      └───┤ 7805   │──→ +5V for Z80 Logic
          │ Reg.   │
          └────────┘
              │
              └──────→ Common GND
```

### Power Consumption Estimate

- Each stepper motor: 12V × 1.5A = 18W
- Two motors: 36W
- Z80 + logic: 5W
- **Total**: ~40-50W
- **Recommended**: 12V 5A power supply (60W)

## Testing Procedure

### 1. Initial Power-Up Test (No Motors)

```assembly
; Test code to verify I/O ports
TEST:
    LD   A, 0001B       ; Pattern: 0001
    OUT  (03H), A       ; Output to Alt port
    HALT
```

**Check:** Measure voltage on L298N inputs with multimeter
- IN1 should be HIGH (~5V)
- IN2, IN3, IN4 should be LOW (0V)

### 2. Single Motor Test

Run `simple-test.z80` with one motor connected:

```bash
# Load and run simple-test.bin
# Motor should step slowly
```

**Observe:**
- Motor should rotate smoothly
- No skipped steps
- No excessive vibration
- No overheating

### 3. Both Motors Test

Run `main.z80` test routines:

```assembly
CALL TEST_BOTH_MOTORS
```

**Verify:**
- Both motors move together
- Coordinated motion
- Position tracking accurate

## Troubleshooting

### Problem: Motors Don't Move

**Check:**
1. Power supply connected and ON
2. Common ground between Z80 and L298N
3. Enable pins (ENA, ENB) are HIGH
4. Motor wires connected correctly
5. I/O port addressing correct

**Test:**
```assembly
; Manually output test pattern
LD   A, 0001B
OUT  (03H), A
; Check with oscilloscope or LED
```

### Problem: Motors Vibrate/Hum But Don't Rotate

**Cause:** Wrong step sequence or too-fast stepping

**Fix:**
1. Increase delay values (SLEW_DELAY)
2. Verify step pattern sequence
3. Check motor wiring (may need to swap coil pairs)

### Problem: Motors Move Wrong Direction

**Fix:**
1. Swap any two motor wires (swaps one coil)
2. Or modify software (swap CW/CCW routines)

### Problem: Motors Lose Steps

**Cause:** Moving too fast, too much load, or insufficient power

**Fix:**
1. Increase STEP_DELAY values
2. Add acceleration/deceleration ramping
3. Check power supply voltage under load
4. Reduce telescope load
5. Use higher-current drivers or motors

### Problem: Erratic Behavior

**Check:**
1. Power supply ripple (add larger capacitors)
2. Ground loops (single-point grounding)
3. Wire length (keep short, twisted pairs)
4. EMI from motors (add 0.1µF caps across motor coils)

## Optional Enhancements

### 1. Limit Switches

```
Limit Switch ──┬── +5V
               │
               └── 10kΩ ─── GND
                    │
                    └───→ Status Port Bit
```

### 2. Position Encoders

Add rotary encoders for closed-loop control:
- HN3806-AB encoders (as mentioned in your project)
- Connect A/B channels to input port
- Use quadrature decoding

### 3. Emergency Stop Button

```
E-Stop (NC) ──┬── +5V
              │
              └── 10kΩ ─── GND
                   │
                   ├───→ Status Port
                   └───→ L298N Enable (via inverter)
```

### 4. Current Sensing

Monitor motor current to detect stalls:
- Use L298N current sense pins
- Feed to ADC or comparator
- Trigger interrupt on over-current

## Safety Considerations

1. **Mechanical**
   - Ensure telescope can move freely
   - Add mechanical stops
   - Test full range of motion manually first

2. **Electrical**
   - Use proper wire gauge (20 AWG minimum for motors)
   - Fuse the power supply
   - Isolated power supplies recommended
   - Keep high current away from logic

3. **Software**
   - Implement soft limits in code
   - Add timeout protection
   - Test extensively before full assembly

## Bill of Materials

| Item | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| NEMA 17 Stepper Motor | 2 | $20 | 200 steps/rev |
| L298N Motor Driver | 2 | $6 | Or equivalent |
| 12V 5A Power Supply | 1 | $15 | Wall adapter |
| 74LS244 Buffer | 1 | $1 | For I/O expansion |
| 10:1 Gearbox | 2 | $40 | Or salvage from equipment |
| Wire, connectors | - | $10 | Misc hardware |
| **Total** | | **~$90** | |

## References

- L298N Datasheet: https://www.st.com/resource/en/datasheet/l298.pdf
- Stepper Motor Basics: https://www.ti.com/lit/an/slva488/slva488.pdf
- Z80 I/O Programming: Z80 CPU User Manual

---

**Next Steps:**
1. Build circuit on breadboard
2. Test with `simple-test.z80`
3. Calibrate step counts
4. Integrate with main telescope mount
