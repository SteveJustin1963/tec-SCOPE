# Simple Optical Gate Motor Control System

## Much Simpler Than SPI/Encoder Version!

This version eliminates:
- ❌ ATtiny85 chips (no programming needed!)
- ❌ SPI communication (no complex protocols!)
- ❌ Expensive HN3806 encoders ($30 each)

Instead uses:
- ✅ Simple optical slot sensors (~$2 each)
- ✅ DIY slotted disk (free - cut from tin lid!)
- ✅ Direct pulse counting in Z80
- ✅ Total cost: ~$25 vs $60

---

## Hardware Requirements

### Components Needed

| Qty | Component | Example Part | Cost |
|-----|-----------|--------------|------|
| 2 | Optical slot sensor | GP1A57HRJ00F, ITR8307, ITR9608 | $4 |
| 2 | L298N motor driver | Standard module | $6 |
| 2 | DC geared motors | 12V with 100:1 reduction | varies |
| 1 | 12V power supply | 2A minimum | $10 |
| 1 | 7805 regulator | +5V output | $1 |
| 2 | 10kΩ resistor | Pull-up for optical sensors | $0.20 |
| 2 | 100nF capacitor | Debouncing | $0.20 |
| 1 | Tin lid | From food can | FREE |
| - | Wire, connectors | Various | $3 |
| **Total** | | | **~$25** |

---

## Making the Slotted Disk (Optical Gate)

### Materials:
- Tin lid from food can (e.g., coffee tin, soup can)
- Tin snips or strong scissors
- Drill
- Marker pen
- Ruler or protractor
- Sandpaper

### Steps:

#### 1. Cut Out the Disk
```
1. Remove tin lid from can
2. Flatten if bent
3. Mark circle with compass or trace round object
4. Recommended diameter: 40-60mm (fits most motors)
5. Cut out circle with tin snips
6. Sand edges smooth (sharp!)
```

#### 2. Mark the 8 Slots
```
8 slots = 360° / 8 = 45° spacing

Method A (with protractor):
- Mark center point
- Mark 8 radial lines every 45°
- Each slot: ~5mm wide at edge

Method B (without protractor):
- Fold disk in half → crease
- Fold in half again → 4 sections
- Fold in half once more → 8 sections
- Cut slots along fold lines
```

#### 3. Cut the Slots
```
Slot dimensions:
- Width: 3-5mm (must be wider than sensor gap)
- Length: 10-15mm (from edge toward center)
- Leave center solid for mounting

Cut pattern (view from above):

         ╱──╲   Slot 1
        ╱    ╲
    ───┘      └───
   ╱              ╲
  │      [O]      │  ← Center mounting hole
   ╲              ╱
    ───┐      ┌───
        ╲    ╱
         ╲──╱   Slot 8

8 equally-spaced slots around circumference
```

#### 4. Mount to Motor Shaft
```
Options:
A. Drill hole in center, mount with setscrew collar
B. Use hot glue (if shaft is flat)
C. 3D print a shaft adapter if available
D. Aluminum tube + hose clamp

Critical: Disk must spin with motor shaft!
```

#### 5. Test Alignment
```
Spin motor by hand slowly
Optical sensor should:
- Output HIGH when metal blocks light
- Output LOW when slot passes (light through)
- Have clean transitions (not wobbling)
```

### Slot Template (actual size for 50mm disk)

```
Print this at 100% scale:

    ╔═══════════════════════════════════════╗
    ║                                       ║
    ║         •              •              ║
    ║    •         •    •         •         ║
    ║   •                           •       ║
    ║  •              [O]             •     ║
    ║   •                           •       ║
    ║    •         •    •         •         ║
    ║         •              •              ║
    ║                                       ║
    ║   Dots = slot positions (8 total)     ║
    ║   [O] = center mounting hole          ║
    ║   Circle diameter = 50mm              ║
    ╚═══════════════════════════════════════╝
```

---

## Optical Sensor Selection & Wiring

### Recommended Sensors

**Best Options (transmissive slot type):**
1. **GP1A57HRJ00F** - Sharp, 3mm slot, 5V
2. **ITR8307** - Everlight, 5mm slot, 5V
3. **ITR9608** - Everlight, 3mm slot, 5V
4. **H21A1** - Honeywell, 3mm slot, 5V

**Cheaper Option (reflective type - requires white marks on disk):**
- **TCRT5000** - IR reflective sensor module (~$1)

### Pinout (typical slot sensor)

```
     ┌─────────────┐
     │   ╭─────╮   │  ← LED side (emitter)
     │   │ LED │   │
     │   ╰─────╯   │
     └──┬───────┬──┘
        │       │
      Anode   Cathode
        │       │
        │    ┌──┴──┐  Gap for slotted disk
        │    │     │
     ┌──┴──┐ │     │
     │  PT │ │     │  ← Phototransistor side (detector)
     └─────┘ │     │
        │    └─────┘
        │       │
    Collector Emitter
```

### Wiring Diagram

```
For EACH optical sensor (2 total: X and Y):

                    +5V
                     │
                     ├───┐
                     │   │
                    ┌┴┐  │
        LED side:   │R│  │  R = 220Ω (current limit for LED)
                    └┬┘  │
                     │   │
                   Anode │
                     │   │
                  ┌──┴───┴──┐
                  │  SLOT   │
Slotted disk ────>│ SENSOR  │
passes through    └──┬───┬──┘
                     │   │
                 Cathode │
                     │   │
                    GND  Collector
                         │
                         ├─── 10kΩ ───┬─── +5V (pull-up)
                         │            │
                         │           ┌┴┐
                         │           │C│  C = 100nF (debounce)
                         │           └┬┘
                         │            │
                         └────────────┴─────► Z80 Port 03h bit 0 (or 1)
                         │            │
                        Emitter      GND
                         │
                        GND


Signal behavior:
- Slot passes: Light hits phototransistor → LOW output
- Metal blocks: No light → Pull-up pulls HIGH
```

### Connection to Z80

```
┌─────────────────────────────────────────┐
│           TEC-1 Z80 SBC                 │
│                                         │
│  Port 03h (Input)                       │
│    bit 0 ◄─── Optical X sensor         │
│    bit 1 ◄─── Optical Y sensor         │
│    bit 2 ◄─── Button X forward         │
│    bit 3 ◄─── Button X reverse         │
│    bit 4 ◄─── Button Y forward         │
│    bit 5 ◄─── Button Y reverse         │
│                                         │
│  Port 04h (Output)                      │
│    bit 0 ───► Motor X direction         │
│    bit 1 ───► Motor Y direction         │
│                                         │
│  Port 05h (Output)                      │
│    8-bit ───► Motor X PWM (via buffer)  │
│                                         │
│  Port 06h (Output)                      │
│    8-bit ───► Motor Y PWM (via buffer)  │
│                                         │
└─────────────────────────────────────────┘
```

---

## Specifications

### Resolution

With 8 slots and 100:1 gear reduction:

```
Motor shaft: 8 pulses/revolution
Gear reduction: 100:1
Telescope: 8 × 100 = 800 pulses/revolution

Angular resolution:
360° / 800 = 0.45° per pulse
             = 27 arc-minutes per pulse

This is acceptable for visual use!
For better resolution, cut more slots (16 slots = 13.5 arc-min)
```

### Comparison to Encoder Version

| Feature | Optical Gate | Quadrature Encoder |
|---------|--------------|-------------------|
| Resolution | 0.45°/count | 0.06°/count |
| Cost | $25 | $60 |
| Complexity | Very simple | Complex |
| Direction sensing | Software only | Hardware |
| Can lose position? | Yes (if Z80 crashes) | Yes (if Z80 crashes) |
| Best for | Visual observing | Astrophotography |

---

## Position Tracking

### How It Works

```
1. Optical sensor outputs LOW when slot passes
2. Z80 detects LOW→HIGH transition (rising edge)
3. Z80 increments or decrements counter based on motor direction
4. Direction is tracked in software (you commanded the motor)

                 Direction = 1 (CW)
                      ↓
    Sensor ──┐   ┌──┐   ┌──┐   ┌──┐
             │   │  │   │  │   │  │
             └───┘  └───┘  └───┘  └───

    Count:   0  1  2  3  4  5  6  7  8
                 ↑
             Increment on rising edge


                 Direction = 0 (CCW)
                      ↓
    Sensor ──┐   ┌──┐   ┌──┐   ┌──┐
             │   │  │   │  │   │  │
             └───┘  └───┘  └───┘  └───

    Count:   8  7  6  5  4  3  2  1  0
                 ↑
             Decrement on rising edge
```

### Limitations

**You might miss counts if:**
1. Z80 is busy doing other tasks (solution: use interrupts)
2. Motor spins too fast (max ~1000 RPM for polling, higher with interrupts)
3. Disk wobbles causing extra pulses (secure disk firmly)

**Position can drift if:**
1. You manually move telescope without motors (no counts generated)
2. Z80 loses power (no battery backup)

**Solutions:**
- Add "home" sensor (extra optical sensor at known position)
- Re-calibrate at startup by pointing at known star
- Add battery backup for position variables

---

## Calibration Procedure

### 1. Set Home Position
```assembly
; Point telescope at known reference (e.g., North horizon)
CALL ZERO_POSITIONS   ; Reset counters to zero
; Now position 0,0 = North, horizon
```

### 2. Verify Count Direction
```
1. Command motor X forward (Alt up)
2. Watch count increase
3. If decreases: swap motor wires or invert DIR bit
```

### 3. Measure Counts Per Degree
```
1. From home, move exactly 90° using bubble level
2. Read position counter
3. Should be ~200 counts (800/360 * 90)
4. If different, adjust your gear ratio or slot count
```

### 4. Test GoTo Accuracy
```assembly
; Test: Point to Alt=45°, Az=180°
LD HL, 100        ; 45° = 100 counts (approx)
CALL SET_TARGET_X

LD HL, 400        ; 180° = 400 counts
CALL SET_TARGET_Y

; Let main loop move telescope
; Check: Did it point to correct position?
```

---

## What You Might Be Missing

### 1. ✅ Direction Sensing
**Issue:** Optical gate doesn't tell you which direction motor is spinning!

**Solution:** Track direction in software based on which direction YOU commanded the motor. Code already does this in `MOTOR_X_DIR` and `MOTOR_Y_DIR` variables.

**Limitation:** If you manually move telescope by hand, counts will be wrong. Fix: re-calibrate or add home sensor.

---

### 2. ✅ Debouncing
**Issue:** Mechanical vibration can cause multiple pulses per slot (contact bounce)

**Solution 1 (Hardware):** Add 100nF capacitor across sensor output (already in BOM)

**Solution 2 (Software):** Add debounce delay in code:
```assembly
; After detecting edge, wait a bit before accepting next edge
READ_EDGE:
        ; Detect edge (already in code)
        ...
        ; Debounce delay
        LD B, 10
DEBOUNCE:
        DJNZ DEBOUNCE
        RET
```

---

### 3. ✅ Missed Pulses
**Issue:** If Z80 is busy, it might miss a pulse when slot passes

**Solutions:**
1. **Use interrupts** (better but more complex)
2. **Poll faster** (call READ_SENSORS more frequently)
3. **Slow down motors** (lower PWM duty cycle)
4. **Add more slots** (each revolution has more chances to count)

**Current code uses polling** - good enough if you don't spin motors too fast.

---

### 4. ✅ Home/Reference Position
**Issue:** If power is lost, you don't know where telescope is pointing

**Solution:** Add a third optical sensor at a fixed "home" position:
```
- Mount extra sensor at Az = 0° position (true North)
- At startup, rotate Az until home sensor triggers
- Set Az counter = 0
- Now you have absolute position reference
```

---

### 5. ✅ Disk Wobble
**Issue:** If slotted disk wobbles or isn't centered, counts will be erratic

**Solutions:**
- Balance the disk (add weight opposite any heavy spots)
- Use thicker material (less flex)
- Mount disk closer to motor bearing (less wobble)
- Secure disk with setscrew, not just glue

**Test:** Spin motor slowly by hand, watch sensor output with LED or multimeter. Should be clean pulses, not flickering.

---

### 6. ⚠️ Light Interference
**Issue:** Ambient light (sunlight, flashlight) can trigger sensor

**Solutions:**
- Use IR optical sensors (less sensitive to visible light)
- Add light shield/tube around sensor
- Use transmissive slot type (harder for external light to interfere)
- Paint disk black (reduces reflections)

**Test:** Shine flashlight at sensor. If it triggers, add shielding.

---

### 7. ✅ Sensor Alignment
**Issue:** Sensor gap must align with disk edge

**Solution:**
```
Correct alignment:

   Motor ━━━●━━━━━━━━━━
          shaft
             │
             ▼
         ╭───┴───╮
     ────│  Disk │──── ← Slots at edge
         ╰───────╯
             │
             ▼
         ┌───┴───┐
         │ ┌───┐ │  ← Sensor gap
         │ └───┘ │
         └───────┘
         Optical
         sensor

- Disk edge must pass through sensor gap
- Gap should be slightly wider than disk thickness
- Check: Disk doesn't rub on sensor
```

---

## Quick Build Checklist

**Hardware:**
- [ ] Cut slotted disk from tin lid (8 equal slots)
- [ ] Sand edges smooth (tin is sharp!)
- [ ] Mount disk on motor shaft (secure, no wobble)
- [ ] Mount optical sensors aligned with disk edge
- [ ] Wire sensors to Port 03h bits 0 & 1
- [ ] Add pull-up resistors (10kΩ) and debounce caps (100nF)
- [ ] Wire motor drivers to Port 04h, 05h, 06h
- [ ] Add manual control buttons (optional)
- [ ] Connect power supplies (12V for motors, 5V for logic)

**Software:**
- [ ] Load `optical-gate-control.z80` to TEC-1 at 0800h
- [ ] Test: Read sensors while rotating disk by hand
- [ ] Test: Motors respond to manual buttons
- [ ] Calibrate: Set home position
- [ ] Test: GoTo positioning works

**Calibration:**
- [ ] Point telescope at known position (North)
- [ ] Call `ZERO_POSITIONS` to reset counters
- [ ] Test by moving to known angle
- [ ] Adjust proportional gain if needed (currently x2)

---

## Advantages of This Simple Design

1. **Much cheaper** - $25 vs $60
2. **No microcontroller programming** - No ATtiny85 to program!
3. **No SPI protocol** - Just simple pulse counting
4. **Easy to debug** - Watch LED on sensor to see it working
5. **DIY friendly** - Make disk from scrap metal
6. **Good enough resolution** - 0.45° is fine for visual observing

## When to Use Encoder Version Instead

Use the quadrature encoder version (SPI with ATtiny85) if:
- You need better resolution (0.06° vs 0.45°)
- You're doing astrophotography (needs precise tracking)
- You want hardware direction sensing (more reliable)
- Cost isn't an issue

## Summary

**You're NOT missing anything!** This simple optical gate design will work great for:
- Visual observing
- Manual telescope control
- Learning about motor control
- Budget builds

The code handles direction tracking, edge detection, and position counting. Just build the slotted disk carefully and align the sensors properly!

Good luck! 🔭
