# Position Accuracy & 3 Decimal Place Display

## Quick Answers

### 1. ✅ YES - Each Motor Goes Forward AND Reverse

Both motors have **full bi-directional control**:

```
Motor X (Altitude):
  Port 04h bit 0 = 1 → Forward (telescope points UP)
  Port 04h bit 0 = 0 → Reverse (telescope points DOWN)

Motor Y (Azimuth):
  Port 04h bit 1 = 1 → Forward (telescope points CLOCKWISE)
  Port 04h bit 1 = 0 → Reverse (telescope points COUNTER-CLOCKWISE)
```

This is handled by the **L298N H-Bridge** driver:
- H-Bridge = "H" shaped circuit that reverses current flow
- Reversing current = reversing motor direction
- Same motor, same speed, just swaps which way it spins!

### 2. ✅ YES - Pulse Counting Gives Exact Position

With your specifications:
- **8 slots** in your tin disk
- **100:1 gear reduction**
- **800 total pulses per 360° rotation**

```
Resolution calculation:
360° ÷ 800 counts = 0.45° per count
                  = 27 arc-minutes per count
                  = 1,620 arc-seconds per count
```

### 3. ✅ YES - We Can Display to 3 Decimal Places

Even though **physical resolution is 0.45°**, we can **calculate and display to 3 decimal places**:

```
Examples:
Count = 0   → Position = 0.000°
Count = 1   → Position = 0.450°
Count = 2   → Position = 0.900°
Count = 100 → Position = 45.000°
Count = 222 → Position = 99.900°
Count = 400 → Position = 180.000°
Count = 800 → Position = 360.000°
```

---

## Understanding Resolution vs Display Precision

### Important Distinction:

**Physical Resolution:** 0.45° (what you can actually measure)
**Display Precision:** 0.001° (what you can calculate and show)

Think of it like a digital thermometer:
- Physical sensor resolution: ±1°C
- Display shows: 23.456°C
- The last two digits (5, 6) are **calculated**, not measured

### For Your Telescope:

```
Count = 123

Physical measurement:
  "Telescope is somewhere between 55.35° and 55.80°"

Calculated position:
  (123 × 360) / 800 = 44,280 / 800 = 55.350°

Display shows:
  "55.350°" ← This is the CENTER of your resolution bin
```

---

## Position Calculation Formula

### Counts → Degrees (with 3 decimal places):

```
degrees = (count × 360.000) / 800
        = count × 0.45

Examples:
  count = 0   → 0 × 0.45 = 0.000°
  count = 100 → 100 × 0.45 = 45.000°
  count = 222 → 222 × 0.45 = 99.900°
  count = 555 → 555 × 0.45 = 249.750°
  count = 800 → 800 × 0.45 = 360.000°
```

### Degrees → Counts (for GoTo commands):

```
count = (degrees × 800) / 360
      = degrees × 2.222...

Examples:
  degrees = 0.000°   → 0 × 2.222 = 0 counts
  degrees = 45.000°  → 45 × 2.222 = 100 counts
  degrees = 90.000°  → 90 × 2.222 = 200 counts
  degrees = 180.000° → 180 × 2.222 = 400 counts
  degrees = 270.000° → 270 × 2.222 = 600 counts
  degrees = 360.000° → 360 × 2.222 = 800 counts
```

---

## Practical Examples

### Example 1: Point to Polaris (Az = 180°, Alt = 45°)

```assembly
; Calculate counts needed for Alt = 45.000°
; count = (45 × 800) / 360 = 36000 / 360 = 100 counts

LD HL, 100
CALL SET_TARGET_X       ; Set Alt target to 100 counts = 45.000°

; Calculate counts for Az = 180.000°
; count = (180 × 800) / 360 = 144000 / 360 = 400 counts

LD HL, 400
CALL SET_TARGET_Y       ; Set Az target to 400 counts = 180.000°

; Motors will automatically move to these positions
; Main loop handles the movement
```

**Display shows:**
```
Alt: 45.000°
Az: 180.000°
```

**Physical accuracy:**
```
Alt: 45.000° ± 0.225° (half of 0.45°)
Az: 180.000° ± 0.225°
```

### Example 2: Point to Sirius (Az = 165.3°, Alt = 23.7°)

```assembly
; Alt = 23.7° → count = (23.7 × 800) / 360 = 52.666... ≈ 53 counts
LD HL, 53
CALL SET_TARGET_X       ; Sets Alt to 53 counts

; Az = 165.3° → count = (165.3 × 800) / 360 = 367.333... ≈ 367 counts
LD HL, 367
CALL SET_TARGET_Y       ; Sets Az to 367 counts
```

**Display shows:**
```
Alt: 23.850° (actual: 53 counts)
Az: 165.150° (actual: 367 counts)
```

**Notice:** Your target was 23.7°, but you can only get to 23.85° (nearest count).
This is called **quantization error** = 0.15° in this case.

**Is this good enough?**
- ✅ For visual observing: YES! Easy to see the object in eyepiece
- ❌ For astrophotography: NO! Star might be at edge of frame

---

## Full Position Lookup Table (All 800 Counts)

### Selected Positions (every 50 counts):

| Count | Degrees | Arc-Min | Notes |
|-------|---------|---------|-------|
| 0 | 0.000° | 0' 0" | North/Horizon reference |
| 50 | 22.500° | 22° 30' | Low in sky |
| 100 | 45.000° | 45° 0' | Polaris altitude (approx) |
| 150 | 67.500° | 67° 30' | High in sky |
| 200 | 90.000° | 90° 0' | Zenith |
| 250 | 112.500° | 112° 30' | Overhead |
| 300 | 135.000° | 135° 0' | |
| 350 | 157.500° | 157° 30' | |
| 400 | 180.000° | 180° 0' | South |
| 450 | 202.500° | 202° 30' | |
| 500 | 225.000° | 225° 0' | Southwest |
| 550 | 247.500° | 247° 30' | |
| 600 | 270.000° | 270° 0' | West |
| 650 | 292.500° | 292° 30' | |
| 700 | 315.000° | 315° 0' | Northwest |
| 750 | 337.500° | 337° 30' | |
| 800 | 360.000° | 360° 0' | Full rotation |

### Every Single Count (first 20):

| Count | Degrees | Fractional |
|-------|---------|-----------|
| 0 | 0.000° | exact |
| 1 | 0.450° | exact |
| 2 | 0.900° | exact |
| 3 | 1.350° | exact |
| 4 | 1.800° | exact |
| 5 | 2.250° | exact |
| 6 | 2.700° | exact |
| 7 | 3.150° | exact |
| 8 | 3.600° | exact |
| 9 | 4.050° | exact |
| 10 | 4.500° | exact |
| 11 | 4.950° | exact |
| 12 | 5.400° | exact |
| 13 | 5.850° | exact |
| 14 | 6.300° | exact |
| 15 | 6.750° | exact |
| 16 | 7.200° | exact |
| 17 | 7.650° | exact |
| 18 | 8.100° | exact |
| 19 | 8.550° | exact |
| 20 | 9.000° | exact |

**Pattern:** Each count = exactly **+0.450°**

---

## Motor Direction & Counting

### Forward Direction (DIR = 1):

```
Motor spins → Disk rotates → Optical sensor sees slots

Slot 1 passes: Count = 0 → 1    (0.000° → 0.450°)
Slot 2 passes: Count = 1 → 2    (0.450° → 0.900°)
Slot 3 passes: Count = 2 → 3    (0.900° → 1.350°)
...
Slot 8 passes: Count = 7 → 8    (3.150° → 3.600°)

One motor revolution complete!
But telescope only moved 1/100th of a revolution due to gearing.

After 100 motor revolutions:
Count = 800 → Telescope completes 360°
```

### Reverse Direction (DIR = 0):

```
Motor spins backwards → Disk rotates backwards

Code DECREMENTS count on each pulse instead of incrementing

Slot passes: Count = 800 → 799  (360.000° → 359.550°)
Slot passes: Count = 799 → 798  (359.550° → 359.100°)
...continues backwards...
```

**This is how you can move in both directions!**

---

## Improving Resolution (Future Upgrades)

If you need better than 0.45°:

### Option 1: More Slots in Disk

| Slots | Resolution | Effort |
|-------|-----------|--------|
| 8 (current) | 0.450° | Easy to cut |
| 16 | 0.225° | Moderate |
| 32 | 0.113° | Difficult |
| 64 | 0.056° | Very difficult |

**Limitation:** Cutting 32+ evenly-spaced slots by hand is nearly impossible!

### Option 2: Higher Gear Ratio

| Ratio | Resolution @ 8 slots | Notes |
|-------|---------------------|-------|
| 100:1 (current) | 0.450° | Your setup |
| 200:1 | 0.225° | Need different gearbox |
| 500:1 | 0.090° | Very slow |
| 1000:1 | 0.045° | Extremely slow |

**Limitation:** Higher ratios make slewing VERY slow!

### Option 3: Quadrature Encoder

Switch to quadrature encoder version:
- 600 pulses/rev × 100:1 × 4 (quadrature) = 240,000 counts
- Resolution: 360° / 240,000 = 0.0015° = 5.4 arc-seconds!
- **Cost:** $60 vs $25
- **Complexity:** Much higher

### Recommendation:

**Start with 8 slots!** Then:
1. If good enough → You're done!
2. If need better → Cut 16 slots (doubles resolution to 0.225°)
3. If still not enough → Upgrade to encoder version

---

## Code Example: Complete Position Display

```assembly
; Main loop - continuously display position

DISPLAY_LOOP:
        ; Read Alt encoder
        LD      HL, (POS_X_LO)          ; Current count
        CALL    COUNT_TO_DEGREES_3DP

        ; Display "Alt: 045.000°"
        LD      A, 'A'
        CALL    SERIAL_OUT
        LD      A, 'l'
        CALL    SERIAL_OUT
        LD      A, 't'
        CALL    SERIAL_OUT
        LD      A, ':'
        CALL    SERIAL_OUT
        LD      A, ' '
        CALL    SERIAL_OUT

        LD      A, (DEGREES_INT)
        CALL    DISPLAY_3DIGIT          ; Shows "045"

        LD      A, '.'
        CALL    SERIAL_OUT

        LD      HL, (DEGREES_DEC)
        CALL    DISPLAY_3DIGIT          ; Shows "000"

        LD      A, '°'
        CALL    SERIAL_OUT

        ; Newline
        LD      A, 13
        CALL    SERIAL_OUT
        LD      A, 10
        CALL    SERIAL_OUT

        ; Now display Az
        LD      HL, (POS_Y_LO)
        CALL    COUNT_TO_DEGREES_3DP

        ; Display "Az:  180.000°"
        ; ... (same pattern) ...

        ; Delay before next update
        LD      BC, 1000
DELAY_LOOP:
        DEC     BC
        LD      A, B
        OR      C
        JR      NZ, DELAY_LOOP

        JR      DISPLAY_LOOP
```

**Output on serial terminal:**
```
Alt: 045.000°
Az:  180.000°
Alt: 045.450°
Az:  180.450°
Alt: 045.900°
Az:  180.900°
...
```

---

## Summary

### ✅ Your Questions Answered:

**1. Can each motor go forward and reverse?**
→ **YES!** L298N H-bridge provides full bi-directional control

**2. By counting pulses with known gearing, can we know position?**
→ **YES!** 8 slots × 100:1 ratio = 800 counts per 360°

**3. Display to 3 decimal places?**
→ **YES!** Formula: degrees = (count × 360) / 800
→ Shows: "123.456°"
→ Physical resolution: ±0.225° (half of 0.45°)

### Position Accuracy:

```
┌─────────────────────────────────────────┐
│ Display:           123.456°             │  ← What you see
│ Calculated:        123.450°             │  ← From count × 0.45
│ Physical accuracy: 123.450° ± 0.225°    │  ← Actual position
│ Count:             274 pulses            │  ← What we measured
└─────────────────────────────────────────┘
```

**For visual observing:** 0.45° resolution is **excellent!**
- Moon diameter: 0.5° (you can center it!)
- Jupiter diameter: 0.01° (you can find it!)
- Most star clusters: 0.5° - 2° (easily visible!)

**Good enough to:**
- ✅ Point accurately to any bright star
- ✅ Find planets easily
- ✅ Navigate between constellations
- ✅ Track objects manually with slow slewing
- ⚠️ Not quite good enough for long-exposure astrophotography (need ~0.01° for that)

You have everything you need! 🎯
