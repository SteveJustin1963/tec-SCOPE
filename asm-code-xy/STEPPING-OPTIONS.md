# Stepper Motor Drive Options

## Overview

Bipolar stepper motors can be driven using different energization patterns, each with tradeoffs in torque, resolution, smoothness, and power consumption. This document explains all options available for your telescope control system.

---

## Current Configuration: FULL-STEP MODE ✅

**As of 2026-01-19**, config.z80 has been updated to use **full-step drive** for maximum torque.

```z80
STEP_0  .EQU    0011B   ; Coil A + B
STEP_1  .EQU    0110B   ; Coil B + A'
STEP_2  .EQU    1100B   ; Coil A' + B'
STEP_3  .EQU    1001B   ; Coil B' + A
```

**Benefits:**
- ✅ 41% more torque than wave drive
- ✅ Better holding power
- ✅ Fewer missed steps under load
- ✅ Same resolution (2000 steps/360°)

**Recommended for:**
- Standard telescope operation
- Any telescope over 5 lbs
- Windy conditions
- Unbalanced mounts

---

## Option 1: Wave Drive (Original, Low Torque)

### Pattern

```z80
STEP_0  .EQU    0001B   ; Coil A only
STEP_1  .EQU    0010B   ; Coil B only
STEP_2  .EQU    0100B   ; Coil A' only
STEP_3  .EQU    1000B   ; Coil B' only
```

### Characteristics

| Property | Value |
|----------|-------|
| Torque | 71% of full-step (base = 100%) |
| Resolution | 2000 steps/360° (0.18°/step) |
| Power consumption | 50% (one coil at a time) |
| Heat generation | Low |
| Smoothness | Moderate |
| Vibration | Moderate |

### Visual Pattern

```
Motor Coils:
     N
   ┌─┴─┐
W──┤   ├──E    A = North coil
   └─┬─┘      B = East coil
     S        A' = South coil
              B' = West coil

Step sequence:
Step 0: N  (0001) → rotor aligns North
Step 1: E  (0010) → rotor aligns East
Step 2: S  (0100) → rotor aligns South
Step 3: W  (1000) → rotor aligns West
```

### Advantages
- ✅ Lowest power consumption (50% of full-step)
- ✅ Coolest operation
- ✅ Simple pattern
- ✅ Suitable for very light loads

### Disadvantages
- ❌ Weakest torque (~30% less than full-step)
- ❌ More prone to missed steps
- ❌ Poor holding power when stationary
- ❌ Not recommended for telescopes

### When to Use
- Battery-powered systems with strict power budget
- Very light telescope (< 3 lbs)
- Bench testing with no load
- Motor driver has current limitations

### How to Enable

Change in config.z80:
```z80
STEP_0  .EQU    0001B
STEP_1  .EQU    0010B
STEP_2  .EQU    0100B
STEP_3  .EQU    1000B
```
Rebuild and test.

---

## Option 2: Full-Step Drive (Current Default, Recommended)

### Pattern

```z80
STEP_0  .EQU    0011B   ; Coil A + B
STEP_1  .EQU    0110B   ; Coil B + A'
STEP_2  .EQU    1100B   ; Coil A' + B'
STEP_3  .EQU    1001B   ; Coil B' + A
```

### Characteristics

| Property | Value |
|----------|-------|
| Torque | 100% (maximum for 4-step) |
| Resolution | 2000 steps/360° (0.18°/step) |
| Power consumption | 100% (two coils at a time) |
| Heat generation | Moderate |
| Smoothness | Moderate |
| Vibration | Moderate |

### Visual Pattern

```
Motor Coils:
     N
   ┌─┴─┐
W──┤   ├──E
   └─┬─┘
     S

Step sequence:
Step 0: NE (0011) → rotor aligns Northeast (A+B)
Step 1: SE (0110) → rotor aligns Southeast (B+A')
Step 2: SW (1100) → rotor aligns Southwest (A'+B')
Step 3: NW (1001) → rotor aligns Northwest (B'+A)
```

### Physics: Why More Torque?

**Vector Addition:**
- One coil: Torque = T
- Two coils at 90°: Torque = √(T² + T²) = √2 × T ≈ 1.41T

**Result:** 41% more torque than wave drive

### Advantages
- ✅ Maximum torque (best for 4-step sequence)
- ✅ Strong holding power
- ✅ Resists missed steps
- ✅ No code changes needed (just 4 constants)
- ✅ Same resolution as wave drive

### Disadvantages
- ⚠️ 2× power consumption vs wave drive
- ⚠️ More heat generation (not a problem for most systems)
- ⚠️ Still has some vibration (fixed by half-stepping)

### When to Use
- **Default choice for telescopes**
- Any telescope over 5 lbs
- Outdoor use (wind resistance)
- Fast slewing
- Tracking applications

### How to Enable

**Already enabled!** This is the current default in config.z80.

If you changed it and want to restore:
```z80
STEP_0  .EQU    0011B
STEP_1  .EQU    0110B
STEP_2  .EQU    1100B
STEP_3  .EQU    1001B
```

---

## Option 3: Half-Stepping (8-Step, Smoothest)

### Pattern

```z80
STEP_0  .EQU    0001B   ; A only
STEP_1  .EQU    0011B   ; A + B
STEP_2  .EQU    0010B   ; B only
STEP_3  .EQU    0110B   ; B + A'
STEP_4  .EQU    0100B   ; A' only
STEP_5  .EQU    1100B   ; A' + B'
STEP_6  .EQU    1000B   ; B' only
STEP_7  .EQU    1001B   ; B' + A
```

### Characteristics

| Property | Value |
|----------|-------|
| Torque | Alternates 100% / 141% |
| Resolution | **4000 steps/360°** (0.09°/step) |
| Power consumption | 75% average |
| Heat generation | Moderate |
| Smoothness | **Excellent** |
| Vibration | **Very low** |

### Visual Pattern

```
Step sequence alternates between detents and between-detents:

Step 0: N    (0001) → detent
Step 1: NE   (0011) → between
Step 2: E    (0010) → detent
Step 3: SE   (0110) → between
Step 4: S    (0100) → detent
Step 5: SW   (1100) → between
Step 6: W    (1000) → between
Step 7: NW   (1001) → between
```

### Advantages
- ✅ **2× resolution** (0.09° per step = 5.4 arc-minutes)
- ✅ Much smoother motion
- ✅ Very low vibration
- ✅ Quieter operation
- ✅ Better at slow speeds (tracking)
- ✅ Less resonance issues

### Disadvantages
- ❌ Requires code changes (not just config)
- ❌ Slightly more complex
- ❌ Variable torque (alternates between 1× and 1.41×)
- ❌ All step calculations must be doubled

### When to Use
- High-precision applications
- Visual astronomy (vibration-sensitive)
- Long exposure astrophotography
- Smooth tracking required
- Worth the effort for serious observing

### How to Enable

**Requires multiple code changes:**

#### Step 1: Update config.z80

```z80
STEP_0  .EQU    0001B
STEP_1  .EQU    0011B
STEP_2  .EQU    0010B
STEP_3  .EQU    0110B
STEP_4  .EQU    0100B
STEP_5  .EQU    1100B
STEP_6  .EQU    1000B
STEP_7  .EQU    1001B
```

#### Step 2: Update STEP_TABLE in config.z80

```z80
STEP_TABLE:
    .DB     STEP_0, STEP_1, STEP_2, STEP_3
    .DB     STEP_4, STEP_5, STEP_6, STEP_7
```

#### Step 3: Modify All Stepping Routines

In stepper-lib.z80, change all four step functions:

**OLD:**
```z80
AND     03H             ; Wrap at 4 (0-3)
```

**NEW:**
```z80
AND     07H             ; Wrap at 8 (0-7)
```

Affects these locations:
- STEP_ALT_CW: line ~64
- STEP_ALT_CCW: line ~102
- STEP_AZ_CW: line ~135
- STEP_AZ_CCW: line ~177

#### Step 4: Update All Step Constants

In config.z80, double all angle-to-step conversions:

**OLD:**
```z80
; 200 steps/rev × 10 gear = 2000 steps / 360°
STEPS_PER_10_DEG .EQU   56      ; 56 / 10 = 5.6 steps/degree
ALT_MAX         .EQU    900     ; 90°
AZ_MAX          .EQU    2000    ; 360°
```

**NEW:**
```z80
; 200 steps/rev × 10 gear × 2 (half-step) = 4000 steps / 360°
STEPS_PER_10_DEG .EQU   112     ; 112 / 10 = 11.2 steps/degree
ALT_MAX         .EQU    1800    ; 90°
AZ_MAX          .EQU    4000    ; 360°
```

#### Step 5: Update Calibration Constants

In config.z80, double all position values:

**OLD:**
```z80
SCP_ALTITUDE    .EQU    189     ; 34° = 189 steps
SCP_AZIMUTH     .EQU    1000    ; 180° = 1000 steps
```

**NEW:**
```z80
SCP_ALTITUDE    .EQU    378     ; 34° = 378 steps
SCP_AZIMUTH     .EQU    2000    ; 180° = 2000 steps
```

#### Step 6: Rebuild and Test

```bash
make clean
make all
```

**Warning:** Thoroughly test all movements after this change!

---

## Option 4: Microstepping (Hardware Upgrade)

### Overview

**Microstepping** requires specialized motor drivers (A4988, DRV8825, TMC2208, etc.) that can vary current in both coils simultaneously to position the rotor at fractional steps.

### Characteristics

| Property | Value |
|----------|-------|
| Torque | 100% at full steps, ~70% at microsteps |
| Resolution | Up to 256 microsteps/step = **512,000 steps/360°** |
| Power consumption | Variable |
| Heat generation | Low to moderate |
| Smoothness | **Excellent** |
| Vibration | **Minimal** |

### Common Microstepping Modes

| Mode | Steps/rev | Steps/360° (with 10:1 gear) | Resolution |
|------|-----------|------------------------------|------------|
| 1/2 | 400 | 4,000 | 0.09° (5.4 arcmin) |
| 1/4 | 800 | 8,000 | 0.045° (2.7 arcmin) |
| 1/8 | 1,600 | 16,000 | 0.0225° (1.35 arcmin) |
| 1/16 | 3,200 | 32,000 | 0.01125° (0.675 arcmin) |
| 1/32 | 6,400 | 64,000 | ~40 arcseconds |

### Hardware Requirements

**Motor Drivers:**
- A4988 (up to 1/16 microstepping, budget option)
- DRV8825 (up to 1/32 microstepping)
- TMC2208 (up to 1/256, ultra-quiet, stealthChop)
- TMC2130 (SPI control, stallGuard, coolStep)

**Connections:**
- MS1, MS2, MS3 pins set microstepping mode
- Step/Direction interface (different from current 4-bit pattern)
- Requires code rewrite for step/dir protocol

### Advantages
- ✅ **Extremely smooth** motion
- ✅ Very high resolution
- ✅ Silent operation (TMC series)
- ✅ Reduced vibration
- ✅ Better for astrophotography

### Disadvantages
- ❌ Requires new motor drivers (~$5-15 each)
- ❌ Different interface (step/dir vs 4-bit pattern)
- ❌ **Major code rewrite required**
- ❌ Lower torque at microstep positions
- ❌ More complex wiring

### How to Enable

**Not compatible with current code!** Would require:

1. New motor drivers (A4988 or DRV8825)
2. Rewiring (step pin + direction pin per motor)
3. Complete rewrite of stepper-lib.z80:
   - Replace 4-bit pattern output with step pulses
   - Add direction control
   - Implement pulse timing

**Recommendation:** Only consider if you need ultra-smooth tracking for astrophotography.

---

## Comparison Table

| Feature | Wave | Full-Step | Half-Step | Microstepping |
|---------|------|-----------|-----------|---------------|
| **Torque** | 71% | 100% | 100%/141% | 70-100% |
| **Steps/360°** | 2000 | 2000 | 4000 | 4000-512000 |
| **Resolution** | 0.18° | 0.18° | 0.09° | 0.09° - 0.0025° |
| **Smoothness** | Moderate | Moderate | Good | Excellent |
| **Vibration** | Moderate | Moderate | Low | Minimal |
| **Power** | 50% | 100% | 75% | Variable |
| **Code Change** | 4 bytes | 4 bytes | Major | Complete rewrite |
| **Hardware Change** | None | None | None | New drivers |
| **Cost** | $0 | $0 | $0 | $10-30 |

---

## Recommendations by Use Case

### Casual Visual Observing
**Use:** Full-step (current default)
- Good torque
- No code changes
- Adequate resolution

### Astrophotography (Short Exposure)
**Use:** Half-step
- Smoother tracking
- Better resolution
- Worth the code effort

### Astrophotography (Long Exposure)
**Use:** Microstepping hardware
- Ultra-smooth tracking
- Minimal vibration
- Professional results

### Portable/Battery Powered
**Consider:** Wave drive
- Lower power consumption
- Acceptable for light scopes
- Test if torque is sufficient

### Heavy Telescope (> 10 lbs)
**Must use:** Full-step or half-step
- Wave drive will miss steps
- Need maximum torque

---

## Testing Procedure After Changes

After changing stepping mode:

### 1. Bench Test (No Load)
```
[ ] Motors turn smoothly
[ ] No stuttering or missed steps
[ ] Position tracking accurate
[ ] Display updates correctly
```

### 2. Light Load Test
```
[ ] Mount small telescope
[ ] Test slewing in all directions
[ ] Check tracking stability
[ ] Verify no missed steps
```

### 3. Full Load Test
```
[ ] Mount full telescope
[ ] Test at various speeds (1x to 9x)
[ ] Check in wind (if outdoor)
[ ] Long tracking test (30+ minutes)
```

### 4. Position Accuracy
```
[ ] Slew to known star
[ ] Check position matches
[ ] Return home
[ ] Repeat 10 times
[ ] Verify cumulative error < 1°
```

---

## Quick Change Guide

### To Switch from Full-Step to Wave Drive

Edit config.z80 lines 69-72:

**Change FROM:**
```z80
STEP_0  .EQU    0011B
STEP_1  .EQU    0110B
STEP_2  .EQU    1100B
STEP_3  .EQU    1001B
```

**Change TO:**
```z80
STEP_0  .EQU    0001B
STEP_1  .EQU    0010B
STEP_2  .EQU    0100B
STEP_3  .EQU    1000B
```

Rebuild: `make clean && make all`

---

### To Switch from Full-Step to Half-Step

**See detailed instructions in "Option 3" section above** - requires multiple file changes.

---

## Motor Current and Heat Management

### Current Draw by Mode

Assuming 1.5A/coil stepper motors:

| Mode | Active Coils | Current Draw | Heat |
|------|--------------|--------------|------|
| Wave | 1 | 1.5A | Low |
| Full-step | 2 | 3.0A | Moderate |
| Half-step | 1-2 | 2.25A avg | Moderate |
| Microstepping | 2 (varied) | Variable | Low-Moderate |

### Heat Management Tips

1. **Ensure adequate ventilation** around motors
2. **Add heat sinks** if motors get hot (> 60°C)
3. **Reduce holding current** when stationary (not implemented)
4. **Use PWM on enable pins** for current limiting
5. **Consider thermal cutoff** for safety

---

## Further Reading

### Stepper Motor Theory
- [Microchip AN907: Stepping Motor Fundamentals](https://ww1.microchip.com/downloads/en/Appnotes/00907a.pdf)
- [TI SLVA488: Stepper Motor Driving](https://www.ti.com/lit/an/slva488/slva488.pdf)

### Motor Drivers
- [Pololu A4988 Carrier](https://www.pololu.com/product/1182)
- [DRV8825 Datasheet](https://www.ti.com/lit/ds/symlink/drv8825.pdf)
- [TMC2208 Silent Stepper](https://www.trinamic.com/products/integrated-circuits/details/tmc2208-la/)

### Telescope Control
- [ASCOM Standards](https://ascom-standards.org/)
- [INDI Library](https://www.indilib.org/)

---

## Conclusion

**Current recommendation: Keep full-step mode** (already configured)

This provides:
- ✅ Excellent torque for most telescopes
- ✅ Zero code changes required
- ✅ Good balance of performance and simplicity

**Consider half-stepping if:**
- You need smoother tracking
- You're doing astrophotography
- You're willing to modify code

**Consider microstepping if:**
- You're doing serious astrophotography
- Budget allows ($30 in drivers)
- You're willing to rewrite motor control code

---

**Last Updated:** 2026-01-19
**Current Config:** Full-step mode (0011b, 0110b, 1100b, 1001b)
**Status:** Production ready ✅
