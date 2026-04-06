# Optical Gate System - Simple Wiring Diagram

## Quick Reference - Port Connections

```
TEC-1 Z80 Ports:
  Port 03h (Input)  - Optical sensors + buttons
  Port 04h (Output) - Motor directions
  Port 05h (Output) - Motor X PWM speed
  Port 06h (Output) - Motor Y PWM speed
```

---

## System Overview - Simplified

```
                        TEC-1 Z80
                            │
              ┌─────────────┼─────────────┐
              │             │             │
           Port 03h      Port 04h   Port 05h/06h
           (Input)      (Direction)    (PWM)
              │             │             │
              ▼             ▼             ▼
        ┌─────────┐   ┌─────────┐   ┌─────────┐
        │ Optical │   │ L298N   │   │ L298N   │
        │ Sensors │──►│ Driver  │──►│ Driver  │
        │  X & Y  │   │   X     │   │   Y     │
        └─────────┘   └────┬────┘   └────┬────┘
              ▲            │             │
              │            ▼             ▼
          Slotted      Motor X       Motor Y
           Disks        (Alt)         (Az)
```

---

## Connection Tables (Much Easier!)

### Table 1: Z80 to L298N Motor Drivers

| Z80 Port | Wire To | L298N Pin | Function |
|----------|---------|-----------|----------|
| Port 04h bit 0 | → | L298N #1 IN1 | Motor X direction |
| Port 04h bit 1 | → | L298N #2 IN3 | Motor Y direction |
| Port 05h | → | L298N #1 ENA | Motor X speed (PWM) |
| Port 06h | → | L298N #2 ENB | Motor Y speed (PWM) |

**Notes:**
- Connect IN2 and IN4 to GND (fixed for single-direction H-bridge)
- All GND pins connected together (common ground)

### Table 2: L298N to Motors

| L298N Driver | Pins | Wire To | Motor |
|--------------|------|---------|-------|
| L298N #1 | OUT1, OUT2 | → | DC Motor X (Alt axis) |
| L298N #2 | OUT3, OUT4 | → | DC Motor Y (Az axis) |

**Notes:**
- Polarity doesn't matter (direction controlled by IN1/IN3)
- If motor spins wrong way, swap OUT wires

### Table 3: Optical Sensors to Z80

| Optical Sensor | Output Pin | Wire To | Z80 Port |
|----------------|------------|---------|----------|
| Sensor X | Signal | → | Port 03h bit 0 |
| Sensor Y | Signal | → | Port 03h bit 1 |

**Notes:**
- Add 10kΩ pull-up resistor from signal to +5V
- Add 100nF capacitor from signal to GND (debounce)

### Table 4: Power Connections

| Component | Power | Connection |
|-----------|-------|------------|
| L298N #1, #2 | +12V | 12V power supply + |
| L298N #1, #2 | GND | Common ground |
| Optical Sensor X, Y | VCC | +5V (via 220Ω for LED) |
| Optical Sensor X, Y | GND | Common ground |
| TEC-1 Z80 | +5V | From 7805 regulator |

---

## Step-by-Step Wiring Guide

### Step 1: Build Power Supply

```
12V Power Supply (2A min)
    │
    ├──► +12V ──► Both L298N modules (+12V pin)
    │
    ├──► +12V ──┐
    │           │
    │      ┌────▼────┐
    │      │  7805   │  Voltage regulator
    │      │  Reg    │
    │      └────┬────┘
    │           │
    │           ├──► +5V ──► TEC-1 (if needed)
    │           ├──► +5V ──► Optical sensors
    │           │
    └──► GND ───┴──► Common GND (all components)
```

**Parts:**
- 7805 voltage regulator (TO-220 package)
- 100µF capacitor on input
- 100µF capacitor on output
- Heatsink for 7805

### Step 2: Wire Optical Sensor X

```
Optical Sensor X (e.g., GP1A57HRJ00F or ITR9608)

LED Side:
    +5V ──[220Ω]── Anode
                    │
                 LED (IR)
                    │
                Cathode ── GND

Phototransistor Side:
    +5V ──[10kΩ]──┬──► To Port 03h bit 0
                  │
             Collector
                  │
          Phototransistor
                  │
              Emitter ── GND
                  │
            [100nF to GND] (debounce capacitor)
```

**How it works:**
- Slot passes → Light hits phototransistor → Output goes LOW
- Metal blocks → No light → Pull-up resistor pulls HIGH

### Step 3: Wire Optical Sensor Y

Same as Sensor X, but connect output to **Port 03h bit 1**

### Step 4: Wire L298N Driver #1 (Motor X)

```
L298N Module #1
┌─────────────────┐
│ Logic Inputs:   │
│   IN1 ◄───────────── Port 04h bit 0 (direction)
│   IN2 ◄───────────── GND (fixed)
│   ENA ◄───────────── Port 05h (PWM speed)
│                 │
│ Motor Outputs:  │
│   OUT1 ────┐    │
│   OUT2 ────┼────┼──► DC Motor X (Alt)
│            │    │
│ Power:      │    │
│   +12V ◄────────┼──── 12V supply
│   GND  ◄────────┼──── Common GND
└─────────────────┘
```

### Step 5: Wire L298N Driver #2 (Motor Y)

Same as Driver #1, but:
- IN3 ← Port 04h bit 1
- IN4 ← GND
- ENB ← Port 06h
- OUT3/OUT4 → Motor Y

### Step 6: Mount Slotted Disks

```
Motor Shaft
    │
    ▼
┌───────┐
│ Disk  │  ← Slotted tin disk (8 slots)
│ with  │     Mounted on motor shaft
│ 8     │     Slots at edge
│ slots │
└───┬───┘
    │
    ▼
┌───────┐
│ ┌───┐ │  ← Optical sensor positioned
│ │   │ │     so disk edge passes through gap
│ └───┘ │
└───────┘
```

### Step 7: Connect Manual Buttons (Optional)

```
For each button:

+5V ──[10kΩ]──┬──► Port 03h bit 2/3/4/5
              │
           Button (normally open)
              │
             GND

Buttons:
  Port 03h bit 2 = Motor X forward
  Port 03h bit 3 = Motor X reverse
  Port 03h bit 4 = Motor Y forward
  Port 03h bit 5 = Motor Y reverse
```

---

## Complete Wiring Checklist

### Power Wiring:
- [ ] 12V supply connected to both L298N +12V pins
- [ ] 12V supply connected to 7805 input
- [ ] 7805 output (+5V) connected to TEC-1 and sensors
- [ ] All GND pins connected together (star ground at power supply)
- [ ] 100µF caps on 7805 input and output
- [ ] Heatsink on 7805

### Motor Driver Wiring:
- [ ] Port 04h bit 0 → L298N #1 IN1
- [ ] Port 04h bit 1 → L298N #2 IN3
- [ ] Port 05h → L298N #1 ENA
- [ ] Port 06h → L298N #2 ENB
- [ ] IN2 → GND
- [ ] IN4 → GND
- [ ] L298N #1 OUT1/OUT2 → Motor X
- [ ] L298N #2 OUT3/OUT4 → Motor Y

### Optical Sensor Wiring:
- [ ] Sensor X LED: +5V → 220Ω → Anode, Cathode → GND
- [ ] Sensor X PT: Collector → 10kΩ → +5V
- [ ] Sensor X PT: Emitter → GND
- [ ] Sensor X PT: Collector → Port 03h bit 0
- [ ] 100nF cap across Sensor X output to GND
- [ ] Repeat for Sensor Y → Port 03h bit 1

### Mechanical:
- [ ] Slotted disk mounted on Motor X shaft
- [ ] Slotted disk mounted on Motor Y shaft
- [ ] Sensor X aligned with Motor X disk
- [ ] Sensor Y aligned with Motor Y disk
- [ ] Disks don't wobble or rub on sensors
- [ ] Motors mounted to telescope Alt and Az axes

---

## Pin-by-Pin Connection List

### TEC-1 Z80 Port 03h (Input):
```
bit 0 ── From Optical Sensor X output
bit 1 ── From Optical Sensor Y output
bit 2 ── From Manual button (X forward) [optional]
bit 3 ── From Manual button (X reverse) [optional]
bit 4 ── From Manual button (Y forward) [optional]
bit 5 ── From Manual button (Y reverse) [optional]
bit 6 ── Unused
bit 7 ── Unused
```

### TEC-1 Z80 Port 04h (Output):
```
bit 0 ── To L298N #1 IN1 (Motor X direction)
bit 1 ── To L298N #2 IN3 (Motor Y direction)
bit 2-7 ── Unused
```

### TEC-1 Z80 Port 05h (Output):
```
8-bit PWM ── To L298N #1 ENA (Motor X speed)
  0x00 = Motor off
  0x80 = Half speed
  0xFF = Full speed
```

### TEC-1 Z80 Port 06h (Output):
```
8-bit PWM ── To L298N #2 ENB (Motor Y speed)
  0x00 = Motor off
  0x80 = Half speed
  0xFF = Full speed
```

---

## Color Code Suggestions (if using colored wire)

| Wire Color | Use For |
|------------|---------|
| Red | +12V power |
| Black | GND (ground) |
| Orange | +5V power |
| Yellow | Motor X connections |
| Green | Motor Y connections |
| Blue | Optical sensor signals |
| White | Direction control signals |
| Purple | PWM signals |

---

## Simplified Breadboard Layout

```
Breadboard Rails:
  Top rail: +5V (from 7805)
  Bottom rail: GND (common ground)

Left Section:
  ┌─────────────────┐
  │ Optical Sensor X│
  │ - LED to +5V    │
  │ - PT to Port 03h│
  └─────────────────┘

Middle Section:
  ┌─────────────────┐
  │ 7805 Regulator  │
  │ Input: 12V      │
  │ Output: 5V      │
  └─────────────────┘

Right Section:
  ┌─────────────────┐
  │ Optical Sensor Y│
  │ - LED to +5V    │
  │ - PT to Port 03h│
  └─────────────────┘

Below breadboard:
  L298N #1 (Motor X driver)
  L298N #2 (Motor Y driver)
```

---

## Testing Sequence

### 1. Power Test
```
With NO motors connected:
[ ] 12V supply shows 11-13V
[ ] 7805 output shows 4.75-5.25V
[ ] No short circuits (use multimeter continuity test)
```

### 2. Sensor Test
```
[ ] Rotate disk by hand slowly
[ ] Sensor output toggles HIGH/LOW on multimeter
[ ] No flickering (disk doesn't wobble)
```

### 3. Motor Test
```
[ ] Port 05h = 0x80, Port 04h bit 0 = 1 → Motor X spins forward
[ ] Port 05h = 0x80, Port 04h bit 0 = 0 → Motor X spins reverse
[ ] Port 06h = 0x80, Port 04h bit 1 = 1 → Motor Y spins forward
[ ] Port 06h = 0x80, Port 04h bit 1 = 0 → Motor Y spins reverse
```

### 4. Integrated Test
```
[ ] Spin Motor X → Sensor X counts increment
[ ] Reverse Motor X → Sensor X counts decrement
[ ] Same for Motor Y
```

---

## Common Mistakes to Avoid

❌ **Don't** connect optical sensor output directly to VCC
   ✅ Use 10kΩ pull-up resistor

❌ **Don't** forget debounce capacitor (100nF)
   ✅ Reduces false triggers from noise

❌ **Don't** use separate power supply grounds
   ✅ Connect ALL grounds together (common ground)

❌ **Don't** swap L298N IN1/IN2 (or IN3/IN4) connections
   ✅ IN1/IN3 = control, IN2/IN4 = GND

❌ **Don't** exceed 2A current on L298N
   ✅ Use bigger motors? Get higher current driver

❌ **Don't** forget heatsink on 7805
   ✅ Gets very hot if supplying > 100mA

---

## Troubleshooting Table

| Problem | Check This | Fix |
|---------|-----------|-----|
| No power | 12V supply | Check fuse, connections |
| No 5V | 7805 wiring | Check input caps, heatsink |
| Sensor always HIGH | LED not lit | Check 220Ω resistor, +5V |
| Sensor always LOW | PT shorted | Check phototransistor wiring |
| Motor doesn't spin | L298N power | Check +12V, GND connections |
| Motor spins wrong way | Polarity | Swap OUT1/OUT2 wires |
| Erratic counting | Noise/wobble | Add 100nF cap, balance disk |
| Counts in wrong direction | Software | Swap increment/decrement in code |

---

## Final Wiring Verification

Before applying power, verify with multimeter:

1. **Continuity Tests:**
   - [ ] All GND points connected together
   - [ ] No shorts between +12V and GND
   - [ ] No shorts between +5V and GND

2. **Resistance Tests:**
   - [ ] +5V to Port 03h bit 0: ~10kΩ (pull-up resistor)
   - [ ] +5V to Port 03h bit 1: ~10kΩ (pull-up resistor)

3. **Voltage Tests (power on, no load):**
   - [ ] 12V supply: 11-13V
   - [ ] 7805 output: 4.75-5.25V
   - [ ] Optical sensor outputs: ~5V when blocked

Good luck with your build! This simplified wiring should be much easier to follow. 🔧
