# TEC-SCOPE Wiring Diagram

## Complete System Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                         TEC-1 Z80 SBC                               │
│                                                                     │
│  Port 03h ──┬─ bit0 (CLK)  ────────────────┬──────────────┐       │
│             ├─ bit1 (MOSI) ─────────X       │              │       │
│             ├─ bit2 (CS1)  ────────────┐    │              │       │
│             ├─ bit3 (CS2)  ─────────┐  │    │              │       │
│             └─ bit4 (MISO) ◄────────┼──┼────┴──────────────┘       │
│                                     │  │                            │
│  Port 04h ──┬─ bit0 (M1_DIR) ──────┼──┼────────────────┐           │
│             └─ bit1 (M2_DIR) ──────┼──┼─────────────┐  │           │
│                                     │  │             │  │           │
│  Port 05h ──── Motor 1 PWM ────────┼──┼──────────┐  │  │           │
│  Port 06h ──── Motor 2 PWM ────────┼──┼───────┐  │  │  │           │
│  Port 07h ◄─── Joystick Input      │  │       │  │  │  │           │
│                                     │  │       │  │  │  │           │
└─────────────────────────────────────┼──┼───────┼──┼──┼──┼───────────┘
                                      │  │       │  │  │  │
                                      │  │       │  │  │  │
        ┌─────────────────────────────┘  │       │  │  │  │
        │  ┌────────────────────────────┘       │  │  │  │
        │  │                                     │  │  │  │
        │  │    ┌──────────────────────────┐    │  │  │  │
        │  │    │   ATtiny85 U2 (Alt Enc)  │    │  │  │  │
        │  │    │                          │    │  │  │  │
        │  └───►│ PB3 (CLK)                │    │  │  │  │
        │       │ PB4 (MISO) ──────────────┼────┘  │  │  │
        └──────►│ PB2 (CS)                 │       │  │  │
                │ PB0 (Enc A) ◄────────────┼───┐   │  │  │
                │ PB1 (Enc B) ◄────────────┼─┐ │   │  │  │
                │ VCC ← +5V    GND ← GND   │ │ │   │  │  │
                └──────────────────────────┘ │ │   │  │  │
                                             │ │   │  │  │
                   ┌─────────────────────────┘ │   │  │  │
                   │  ┌────────────────────────┘   │  │  │
                   │  │                            │  │  │
        ┌──────────┼──┼────────────────────────┐   │  │  │
        │  HN3806  │  │   Encoder 1 (Alt)      │   │  │  │
        │          │  │                        │   │  │  │
        │  Green A ┼──┘  White B ───────────────┘  │  │  │
        │  Red +5V        Black GND                │  │  │
        └──────────────────────────────────────────┘  │  │
                                                       │  │
        ┌──────────────────────────────────────────┐  │  │
        │   ATtiny85 U5 (Az Enc)                   │  │  │
        │                                          │  │  │
        │ PB3 (CLK) ◄──────────────────────────────┼──┘  │
        │ PB4 (MISO) ──────────────────────────────┼─────┘
        │ PB2 (CS) ◄───────────────────────────────┼─────┐
        │ PB0 (Enc A) ◄────────────────────────────┼───┐ │
        │ PB1 (Enc B) ◄────────────────────────────┼─┐ │ │
        │ VCC ← +5V    GND ← GND                   │ │ │ │
        └──────────────────────────────────────────┘ │ │ │
                                                     │ │ │
                   ┌─────────────────────────────────┘ │ │
                   │  ┌────────────────────────────────┘ │
                   │  │                                   │
        ┌──────────┼──┼───────────────────────────┐      │
        │  HN3806  │  │   Encoder 2 (Az)          │      │
        │          │  │                           │      │
        │  Green A ┼──┘  White B ──────────────────┘     │
        │  Red +5V        Black GND                      │
        └────────────────────────────────────────────────┘
                                                          │
                                                          │
        ┌─────────────────────────────────────────────────┘
        │
        │      ┌─────────────────────────────────┐
        │      │      L298N Motor Driver 1       │
        │      │                                 │
        └─────►│ IN1 (Direction)                 │
               │ IN2 ← GND                       │
        ┌─────►│ ENA (PWM Enable) ◄──[Transistor]──── From Port 05h
        │      │ OUT1 ────┐                      │
        │      │ OUT2 ────┼──► DC Motor 1 (Alt) │
        │      │ +12V  GND│                      │
        │      └──────────┼──────────────────────┘
        │                 │
        │                 └──────┐
        │                        │
        │      ┌─────────────────┼──────────────┐
        │      │      L298N Motor Driver 2      │
        │      │                 │              │
        └─────►│ IN3 (Direction) │              │
               │ IN4 ← GND       │              │
        ┌─────►│ ENB (PWM Enable) ◄─[Transistor]──── From Port 06h
        │      │ OUT3 ────┐      │              │
        │      │ OUT4 ────┼──────┼──► DC Motor 2 (Az)
        │      │ +12V  GND│      │              │
        │      └──────────┼──────┼──────────────┘
        │                 │      │
        │                 │      │
        │        Motor 1 ─┘      └─ Motor 2
        │        (Altitude)         (Azimuth)
        │
        │
        │      ┌─────────────────────────────────┐
        │      │   PWM Interface Circuit         │
        │      │   (For each motor)              │
        │      │                                 │
        │      │   Z80 Port ──┬── 10kΩ ──┬── +5V│
        │      │              │          │      │
        │      │              └─ 2N2222 ──┴── GND│
        │      │                   │            │
        │      │                   └──► L298N EN│
        │      └─────────────────────────────────┘
```

## Detailed Component Connections

### 1. ATtiny85 Encoder Interface (x2)

#### ATtiny85 Pin Connections
```
ATtiny85 Pin    Function        Connection
────────────────────────────────────────────────────
Pin 1 (PB5)     RESET          +5V via 10kΩ pull-up
Pin 2 (PB3)     SPI CLK        Z80 Port 03h bit 0
Pin 3 (PB4)     SPI MISO       Z80 Port 03h bit 4 (with 1kΩ resistor)
Pin 4 (GND)     Ground         Common GND
Pin 5 (PB0)     Encoder A      HN3806 Green wire (via 10kΩ pull-up)
Pin 6 (PB1)     Encoder B      HN3806 White wire (via 10kΩ pull-up)
Pin 7 (PB2)     Chip Select    Z80 Port 03h bit 2 (U2) or bit 3 (U5)
Pin 8 (VCC)     Power          +5V
```

**Important:**
- U2 (Alt encoder): CS connected to Port 03h bit 2
- U5 (Az encoder): CS connected to Port 03h bit 3
- Both MISO outputs connected together with 1kΩ series resistors

### 2. HN3806-AB Encoder Connections

```
Encoder Wire    ATtiny85 Pin    Notes
──────────────────────────────────────────────────
Red (+5V)       VCC via 100Ω    Current limiting
Black (GND)     GND             Common ground
Green (A)       PB0             Via 10kΩ pull-up to +5V
White (B)       PB1             Via 10kΩ pull-up to +5V
```

**Critical:** Use pull-up resistors! Do NOT connect A/B directly to VCC!

### 3. L298N Motor Driver Connections

#### Motor Driver 1 (Altitude)
```
L298N Pin       Connection          Notes
──────────────────────────────────────────────────
IN1             Port 04h bit 0      Direction control
IN2             GND                 Fixed for single direction control
ENA             Via transistor      PWM from Port 05h
OUT1            Motor 1 +           DC motor positive
OUT2            Motor 1 -           DC motor negative
+12V            12V supply          Motor power supply
GND             Common GND          All grounds connected
+5V             +5V (optional)      Logic supply (if jumper removed)
```

#### Motor Driver 2 (Azimuth)
```
L298N Pin       Connection          Notes
──────────────────────────────────────────────────
IN3             Port 04h bit 1      Direction control
IN4             GND                 Fixed for single direction control
ENB             Via transistor      PWM from Port 06h
OUT3            Motor 2 +           DC motor positive
OUT4            Motor 2 -           DC motor negative
+12V            12V supply          Motor power supply
GND             Common GND          All grounds connected
+5V             +5V (optional)      Logic supply (if jumper removed)
```

### 4. PWM Interface Transistor Circuit (x2)

For each motor, if L298N ENA/ENB needs higher current than Z80 can provide:

```
               +5V
                │
                ├─── 10kΩ ────┐
                │             │
Z80 Port ──────┴──────┐      │
(05h or 06h)          │      │
                    ┌─┴─┐    │
                    │   │    │
                    │ 2N│    │
                    │2222    │
                    │   │    │
                    └─┬─┘    │
                      │      │
                      ├──────┘
                      │
                      └────────► L298N ENA/ENB
                      │
                     GND
```

**Alternative:** If L298N accepts 5V TTL directly, connect Port 05h/06h straight to ENA/ENB.

### 5. Power Supply

```
┌─────────────────────────────────────┐
│  12V Power Supply (for motors)      │
│  - Minimum 2A (more for bigger      │
│    motors)                          │
│  + ──────┬──► L298N +12V (both)    │
│  - ──────┴──► Common GND            │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│  5V Regulated Supply (for logic)    │
│  - 7805 voltage regulator           │
│  - Input: 12V (from motor supply)   │
│  - Output: 5V for:                  │
│    * TEC-1 (if needed)              │
│    * Both ATtiny85s                 │
│    * Encoders                       │
│  + ──────┬──► All VCC pins          │
│  - ──────┴──► Common GND            │
└─────────────────────────────────────┘
```

### 6. Input Port (Optional - Manual Control)

```
Port 07h Bit    Button/Switch       Function
──────────────────────────────────────────────────
Bit 0           Button 1            Motor 1 Forward
Bit 1           Button 2            Motor 1 Reverse
Bit 2           Button 3            Motor 2 Forward
Bit 3           Button 4            Motor 2 Reverse
Bit 4-7         Reserved            Future use

Each button: One side to GND, other side to port bit
Use 10kΩ pull-up resistors on port bits
```

## Bill of Materials

| Qty | Component              | Notes                          | Est. Cost |
|-----|------------------------|--------------------------------|-----------|
| 2   | ATtiny85               | DIP-8 package                  | $2        |
| 2   | HN3806-AB encoder      | 600 PPR rotary encoder         | $30       |
| 2   | L298N module           | Dual H-bridge driver           | $6        |
| 2   | DC geared motor        | 12V with worm gear preferred   | varies    |
| 1   | 12V power supply       | 2A minimum                     | $10       |
| 1   | 7805 regulator         | +5V output with heatsink       | $1        |
| 8   | 10kΩ resistor          | Pull-ups for encoders/buttons  | $0.50     |
| 2   | 1kΩ resistor           | MISO bus isolation             | $0.10     |
| 2   | 100Ω resistor          | Encoder power limiting         | $0.10     |
| 2   | 2N2222 transistor      | PWM buffering (if needed)      | $0.20     |
| 4   | 100nF capacitor        | Decoupling for ICs             | $0.40     |
| 2   | 100µF capacitor        | Power supply filtering         | $0.50     |
| 1   | Breadboard/PCB         | Prototyping or permanent       | $5        |
| -   | Wire, connectors       | Various                        | $5        |

**Total (excluding motors): ~$60**

## Grounding Notes

**CRITICAL:** All grounds must be connected together:
- Z80 SBC ground
- ATtiny85 grounds (both)
- Encoder grounds (both)
- L298N grounds (both)
- 12V power supply ground
- 5V regulator ground

**Star grounding recommended:** Connect all grounds to a single point near the power supply to minimize ground loops.

## Shielding for Encoders

To reduce noise on encoder signals:
1. Use twisted pair wire for each encoder (twist A+GND, twist B+GND)
2. Keep encoder wires away from motor power wires
3. Add 100nF capacitors across encoder A and B pins to ground at the ATtiny85
4. Use shielded cable if interference persists

## Testing Points

Use a multimeter or oscilloscope to verify:

| Test Point          | Expected Signal                    |
|---------------------|------------------------------------|
| Port 03h bit 0      | Clock pulses during encoder read   |
| Port 03h bit 2      | Low during Enc1 read, high idle    |
| Port 03h bit 3      | Low during Enc2 read, high idle    |
| Port 03h bit 4      | Data pulses during encoder read    |
| Port 04h bit 0      | High/low based on M1 direction     |
| Port 04h bit 1      | High/low based on M2 direction     |
| Port 05h            | PWM waveform (~1kHz)               |
| Port 06h            | PWM waveform (~1kHz)               |
| ATtiny85 PB0/PB1    | Quadrature pulses when encoder turns |
| L298N OUT1/OUT2     | Voltage proportional to PWM        |

## Safety Features to Add

1. **Limit Switches:** Add mechanical limit switches at telescope motion limits
2. **Emergency Stop:** Add a button that disables all motor power
3. **Fuses:** Add fuses on 12V motor power supply
4. **Indicator LEDs:** Add LEDs to show motor direction and power status
5. **Timeout:** Implement software timeout to stop motors if no command received
