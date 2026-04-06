# TEC-1 Telescope Control System - User Manual

**Version 1.1** | **Last Updated: 2026-01-19**

---

## Table of Contents

1. [Quick Start Guide](#quick-start-guide)
2. [First Time Setup](#first-time-setup)
3. [Basic Operations](#basic-operations)
4. [Keypad Controls](#keypad-controls)
5. [Common Tasks](#common-tasks)
6. [Calibration](#calibration)
7. [Troubleshooting](#troubleshooting)
8. [Reference Tables](#reference-tables)

---

## Quick Start Guide

### What You Need

**Hardware:**
- TEC-1 Z80 Single Board Computer
- 2× NEMA 17 stepper motors (200 steps/rev)
- 2× L298N motor drivers
- 12V 5A power supply
- Telescope mount with 10:1 gear reduction
- All wiring complete (see HARDWARE.md)

**Software:**
- `telescope.bin` loaded to TEC-1 at address 0000h

### Power On Sequence

1. **Power on** the 12V supply (motors and drivers)
2. **Power on** the TEC-1
3. **Execute** from address 0000h
4. Display will be blank - system is ready
5. Press **AD** to begin

**That's it!** The system is now waiting for your commands.

---

## First Time Setup

### Step 1: Check Hardware

Before using the system, verify:

**Motors:**
- [ ] Both motors connected to L298N drivers
- [ ] Motor wires secure (red, green, blue, yellow)
- [ ] Motors can turn freely (no binding)

**Power:**
- [ ] 12V supply connected to both L298N modules
- [ ] Common ground between TEC-1 and drivers
- [ ] TEC-1 powered separately (don't share motor 12V)

**Mount:**
- [ ] Telescope can move in both axes
- [ ] No obstructions in movement path
- [ ] Gears engage smoothly

### Step 2: Initial Test (No Load)

Run the simple test program first:

1. Load `simple-test.bin` to TEC-1
2. Execute from 0000h
3. Watch motors step through test sequence:
   - Altitude motor CW 100 steps
   - Altitude motor CCW 100 steps
   - Azimuth motor CW 100 steps
   - Both motors together 100 steps

**Expected:** Motors turn smoothly, no stuttering

**If motors don't move:** See [Troubleshooting](#troubleshooting)

### Step 3: Load Main Program

1. Load `telescope.bin` to TEC-1 address 0000h
2. Execute from 0000h
3. Display blank - ready for input
4. Press **AD** - should show "ALT   "
5. If you see "ALT   ", system is working!

### Step 4: First Movement Test

Let's move the altitude motor 10° upward:

```
Press: AD
Display: "ALT   "

Press: +
Display: "PLUS  "

Press: 1 0
Display: "    10"

Press: GO
Display: "    10"

Press: GO
Display: (starts counting down)
Motor: Moves upward 10°
```

**Success!** You're ready to use the system.

---

## Basic Operations

### Understanding the Two Modes

The system has two input modes:

#### MODE 1: Precision (Deg/Min/Sec) - For Astronomy

Use this when you have coordinates from a star chart.

**Example:** Point to 45° 30' 15"
```
AD → + → 45 GO → 30 GO → 15 GO
```

**When to use:**
- Pointing to stars (you have declination/altitude)
- Precise positioning
- Following star charts

#### MODE 2: Fast (Step Count) - For Manual Control

Use this for quick adjustments and rough positioning.

**Example:** Move 5000 steps at 3× speed
```
F → AD → + → 5000 GO → 3 GO
```

**When to use:**
- Finding targets in the sky
- Bringing objects into view
- Quick adjustments
- When you don't know exact coordinates

### The Display

**6-digit 7-segment display** shows:

**During entry:**
- Text messages: "ALT   ", "AZZ   ", "PLUS  ", etc.
- Numbers you type: "    45", "   180"

**During movement:**
- Countdown of remaining steps
- Shows progress toward target

**Display blank:** System idle, ready for input

---

## Keypad Controls

### The TEC-1 Keypad Layout

```
┌───┬───┬───┬───┐
│ 0 │ 1 │ 2 │ 3 │  Numbers: Enter values
├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ 7 │  Hex digits work too
├───┼───┼───┼───┤
│ 8 │ 9 │ A │ B │  A-F available
├───┼───┼───┼───┤
│ C │ D │ E │ F │  Special functions below
├───┼───┼───┼───┤
│ + │ - │GO │AD │  Control keys
└───┴───┴───┴───┘
```

### Key Functions

| Key | Function | When to Press |
|-----|----------|---------------|
| **AD** | Toggle ALT/AZZ | Start of each command |
| **F** | Fast mode | When entering step counts |
| **+** | Positive direction | After selecting axis |
| **-** | Negative direction | After selecting axis |
| **GO** | Confirm/Next | After entering each field |
| **GO GO** | Execute immediately | Skip remaining fields |
| **C** | Skip field (enter 0) | When field should be 0 |
| **E** | Emergency stop | Anytime - halts motors |
| **0-9** | Enter digits | During number entry |

### Special Keys

**C Key** - Skip to next field
- Use when a field should be zero
- Example: `C GO` = skip degrees, enter minutes

**E Key (RS)** - Emergency Reset
- **Immediately halts both motors**
- Preserves position (doesn't zero)
- Returns to idle state
- Use if wrong target entered or need to stop

**GO GO** (Double-tap) - Quick Execute
- Pressing GO twice executes immediately
- Remaining fields default to 0
- Example: `45 GO GO` = 45° 0' 0"

---

## Common Tasks

### Task 1: Point to a Specific Altitude

**Goal:** Point telescope to 60° altitude

**Steps:**
```
AD          → Display: "ALT   "
+           → Display: "PLUS  "
6 0 GO      → Display: "    60"
GO          → Executes (degrees only, 0' 0")
```

**Shortcut:** `AD → + → 60 GO GO` (double-GO skips min/sec)

---

### Task 2: Point to Specific Azimuth

**Goal:** Point telescope south (180°)

**Steps:**
```
AD          → Display: "ALT   "
AD          → Display: "AZZ   " (toggled)
+           → Display: "PLUS  "
1 8 0 GO    → Display: "   180"
GO          → Executes
```

**Note:** First AD shows "ALT", second AD toggles to "AZZ"

---

### Task 3: Enter Full Coordinates (Deg/Min/Sec)

**Goal:** Point to 45° 30' 15" altitude

**Steps:**
```
AD          → Display: "ALT   "
+           → Display: "PLUS  "
4 5 GO      → Display: "    45" (degrees)
3 0 GO      → Display: "    30" (minutes)
1 5 GO      → Display: "    15" (seconds) → Executes
```

**Result:** Telescope moves to 45° 30' 15" altitude

---

### Task 4: Fast Manual Adjustment

**Goal:** Move azimuth 5000 steps quickly

**Steps:**
```
F           → Display: "FAST  "
AD          → Display: "AZZ   "
+           → Display: "PLUS  "
5 0 0 0 GO  → Display: "  5000"
3 GO        → Executes at 3× speed
```

**Speed options:** 1-9
- 1 = normal (slow, precise)
- 3-5 = good for rough positioning
- 9 = fastest (only for light loads)

---

### Task 5: Scan the Sky

**Goal:** Sweep azimuth slowly to find target

**Steps:**
```
F           → "FAST  "
AD          → "AZZ   "
+           → "PLUS  "
1 0 0 GO    → "   100" (100 steps)
1 GO        → Moves 100 steps at normal speed
```

**Repeat:** Keep pressing `+ → 100 GO → 1 GO` to step across sky

**Or:** Use larger values like 500 steps to move faster

---

### Task 6: Go Home (Zero Position)

**Goal:** Return telescope to starting position (0°, 0°)

**Method 1: Zero Altitude**
```
AD → + → 0 GO GO
```
This moves altitude to 0°, but azimuth stays where it is.

**Method 2: Zero Both Axes**
```
AD → + → 0 GO GO        (zero altitude)
AD → AD → + → 0 GO GO   (zero azimuth)
```

**Note:** The system tracks position internally. "Home" is wherever you initialized (usually horizon, north).

---

### Task 7: Emergency Stop

**Scenario:** Started wrong slew, need to abort immediately

**Action:** Press **E** key

**Result:**
- Both motors halt instantly
- Position preserved (not zeroed)
- System returns to idle
- Ready for new command

**Important:** Position counters still know where telescope is pointing!

---

### Task 8: Skip Fields with C

**Goal:** Move only arc-minutes, skip degrees and seconds

**Steps:**
```
AD          → "ALT   "
+           → "PLUS  "
C           → (skip degrees = 0)
3 0 GO      → "    30" (30 arc-minutes)
GO          → Executes +0° 30' 0"
```

**Use C to skip any field you want to be zero**

---

## Calibration

### Why Calibrate?

Calibration aligns the telescope with true celestial coordinates. Without it:
- ❌ "North" might not be true north
- ❌ Altitude 0° might not be horizon
- ❌ Coordinates won't match star charts

After calibration:
- ✅ Accurate pointing to celestial coordinates
- ✅ Tracking works correctly
- ✅ Can find objects from star charts

### Simple 3-Point Calibration (Recommended)

#### Step 1: Point North at Horizon

1. Manually point telescope **north** at **horizon**
   - Use compass (account for magnetic declination)
   - Sydney: Point to 168° magnetic (12° correction)

2. Reset system:
   - Power cycle TEC-1
   - Or: note current position and zero it

#### Step 2: Level the Altitude

1. Point telescope at **horizon** (altitude 0°)
2. System already knows this is 0° if you just powered on

#### Step 3: Find a Known Star

1. Look up a bright star (Canopus, Achernar, Sirius)
2. Note its current altitude and azimuth
3. Move telescope using keypad to match coordinates
4. Fine-tune by hand until star is centered
5. This validates your calibration

### Advanced Calibration (Using Built-in Routines)

**Note:** These functions exist in main.z80 but aren't directly accessible from keypad. Would need to call them programmatically or add keypad shortcuts.

Available functions:
- `CALIBRATE_POLAR_ALIGNMENT`
- `ALIGN_TO_SOUTH_HORIZON`
- `ALIGN_TO_SCP` (South Celestial Pole)
- `ALIGN_TO_CANOPUS`
- `STORE_TRUE_SOUTH`

**Future enhancement:** Add keypad menu to access these.

### Sydney-Specific Calibration Notes

**Geographic Location:**
- Latitude: 34°S
- Southern hemisphere

**South Celestial Pole:**
- Altitude: 34° (equals latitude)
- Azimuth: 180° (due south)
- Near Sigma Octantis (faint star)

**Magnetic Declination:**
- 12° East
- True North = 168° magnetic bearing
- Compass correction needed

**Bright Reference Stars:**
- Canopus: Brightest southern star (-0.72 mag)
- Achernar: Very bright (0.45 mag), far south
- Sirius: Brightest star overall (-1.46 mag)

---

## Troubleshooting

### Problem: Motors Don't Move

**Check:**
1. ☐ 12V power supply ON and connected
2. ☐ L298N enable pins (ENA, ENB) connected to +5V
3. ☐ Common ground between TEC-1 and drivers
4. ☐ Motor wires connected (4 per motor)
5. ☐ Correct ports: Alt=03h, Az=04h

**Test:**
- LED on L298N should light when energized
- Measure 5V on IN1-IN4 pins during movement
- Check motor resistance: 1-10Ω between coil pairs

**Fix:**
- Verify wiring per HARDWARE.md
- Check TEC-1 I/O ports with oscilloscope
- Try `simple-test.bin` first

---

### Problem: Motors Vibrate But Don't Rotate

**Likely Cause:** Wrong step sequence or too fast

**Fix:**
1. **Slow down:** Increase SLEW_DELAY in config.z80
   ```z80
   SLEW_DELAY  .EQU    100    ; Was 50, now slower
   ```

2. **Check motor wiring:**
   - Swap two wires if needed (swaps one coil)
   - Verify coil pairs with multimeter

3. **Reduce load:**
   - Test without telescope first
   - Balance telescope on mount
   - Check for binding in gears

**Also check:** Speed multiplier in F mode - use 1 or 2, not 9

---

### Problem: Display Shows "ERR  "

**Cause:** Missing +/- direction key

**Fix:** Always press + or - after selecting ALT or AZZ

**Correct sequence:**
```
AD → + → (numbers) → GO
```

**Wrong sequence (causes error):**
```
AD → (numbers) → GO    ← Missing +/-
```

---

### Problem: One Axis Doesn't Move, Other Does

**Before fix:** This was a bug (SLEW early exit)

**After fix (v1.1):** Both axes should complete

**If still happening:**
1. Verify you have v1.1 code (check CHANGES-SUMMARY.md)
2. Rebuild from source
3. Check if one motor is disconnected
4. Test each axis individually:
   ```
   AD → + → 10 GO GO        (test altitude)
   AD → AD → + → 10 GO GO   (test azimuth)
   ```

---

### Problem: Wrong Direction

**Cause:** Motor wiring reversed

**Fix:** Swap any two wires of one coil
- Try swapping red ↔ green (coil A)
- Or swap blue ↔ yellow (coil B)

**Software fix (alternative):**
- Change CW/CCW function calls in code
- Or negate direction in keypad handler

---

### Problem: Position Drift Over Time

**Causes:**
- Missed steps (too fast)
- Load too heavy
- Gear backlash
- No encoder feedback

**Fix:**
1. **Slow down movement:**
   - Use speed 1-2, not 3-9
   - Increase SLEW_DELAY constant

2. **Reduce load:**
   - Balance telescope better
   - Remove heavy accessories
   - Use counterweights

3. **Check for missed steps:**
   - Listen for skipping sounds
   - Watch for jerky motion
   - If missing steps, slower speed needed

4. **Recalibrate periodically:**
   - Use known star to check position
   - Reset if drift > 1°

---

### Problem: Display Blank or Garbled

**Fix:** Press **E** key to reset

**If still blank:**
- System is idle - press AD to start
- Check TEC-1 display ports 01h/02h
- Power cycle TEC-1

**If garbled after E:**
- Power cycle required
- May be NMI timing issue
- Check keypad connections

---

### Problem: Can't Stop Movement

**Emergency:** Press **E** key (RS)

**If E doesn't work:**
1. Power off 12V supply (motors stop)
2. Power off TEC-1
3. Check wiring and restart

**Prevention:** Always test with slow speeds first

---

### Problem: Telescope Hits Mechanical Stop

**Cause:** Limit checking should prevent this (v1.1+)

**If happens:**
1. Immediately press **E** or power off
2. Manually move telescope away from stop
3. Verify you have v1.1 code with limits
4. Check ALT_MAX (900) and AZ_MAX (2000) in config.z80

**Should not happen if:**
- Using v1.1 code
- Altitude stays 0-90°
- Position tracking accurate

---

## Reference Tables

### Key Quick Reference

| Key | Function |
|-----|----------|
| AD | Toggle ALT ↔ AZZ |
| F | Fast mode |
| + | Positive direction |
| - | Negative direction |
| GO | Confirm / Execute |
| C | Skip field |
| E | Emergency stop |

### Degrees to Steps

| Degrees | Steps | Arc-minutes |
|---------|-------|-------------|
| 0.18° | 1 | 10.8' |
| 1° | 5.56 | 60' |
| 10° | 56 | 600' |
| 45° | 250 | 2700' |
| 90° | 500 | 5400' |
| 180° | 1000 | 10800' |
| 360° | 2000 | 21600' |

### Common Positions (Sydney)

| Object | Altitude | Azimuth | Alt Steps | Az Steps |
|--------|----------|---------|-----------|----------|
| Horizon North | 0° | 0° | 0 | 0 |
| Horizon East | 0° | 90° | 0 | 500 |
| Horizon South | 0° | 180° | 0 | 1000 |
| Horizon West | 0° | 270° | 0 | 1500 |
| Zenith | 90° | - | 500 | - |
| SCP | 34° | 180° | 189 | 1000 |

### Speed Multiplier Guide (F Mode)

| Speed | Use Case | Risk |
|-------|----------|------|
| 1 | Precise positioning | None |
| 2 | Normal slewing | Low |
| 3-4 | Fast slewing | Moderate |
| 5-7 | Rough positioning | High |
| 8-9 | Unloaded testing only | Very High |

**Recommendation:** Use 1-2 for real observing

### Input Mode Comparison

| Feature | Mode 1 (AD) | Mode 2 (F) |
|---------|-------------|------------|
| **Entry** | Deg/Min/Sec | Step count |
| **Precision** | High (arc-seconds) | Medium (steps) |
| **Speed** | Fixed (SLEW_DELAY) | Variable (1-9×) |
| **Use case** | Astronomy | Manual control |
| **Star charts** | ✅ Direct match | ❌ Conversion needed |

---

## Tips and Best Practices

### For Beginners

1. **Start slow** - Use speed 1 until comfortable
2. **Test without telescope** - Verify motors work first
3. **Use simple targets** - Point to horizon, zenith
4. **Check each axis** - Test Alt and Az separately
5. **Know your emergency stop** - E key always available

### For Astronomy

1. **Calibrate first** - Take time to align properly
2. **Use Mode 1** - Enter deg/min/sec from charts
3. **Fine-tune by hand** - Get close with motors, perfect manually
4. **Check periodically** - Recalibrate if drift > 1°
5. **Note bright stars** - Use for reference alignment

### For Photography

1. **Balance telescope** - Reduces missed steps
2. **Use slow speeds** - Speed 1 only
3. **Consider half-step** - See STEPPING-OPTIONS.md
4. **Test tracking** - Long exposure will show drift
5. **Monitor closely** - Check alignment every 15 min

### Power Management

1. **Separate supplies** - Don't share motor 12V with logic
2. **Common ground** - Essential for proper operation
3. **Fuse protection** - 5A fuse on 12V supply
4. **Monitor heat** - Motors and drivers can get warm
5. **Duty cycle** - Let motors cool if continuous use

---

## Getting Help

### Documentation Files

| File | What It Contains |
|------|------------------|
| **USER-MANUAL.md** | This file - how to use the system |
| **QUICK-REFERENCE.md** | Command reference card |
| **KEYPAD-INTERFACE.md** | Detailed keypad system explanation |
| **HARDWARE.md** | Wiring and connections |
| **TROUBLESHOOTING.md** | Problem solving guide |
| **STEPPING-OPTIONS.md** | Motor drive modes explained |

### Technical Documentation

| File | What It Contains |
|------|------------------|
| **README.md** | Project overview |
| **SYSTEM-OVERVIEW.md** | Architecture details |
| **config.z80** | Configuration constants |
| **stepper-lib.z80** | Motor control functions |
| **main.z80** | Main program source |

### Bug Reports

Current version: **1.1** (Bug fixes applied 2026-01-19)

Known issues: See IMPLEMENTATION-STATUS.md

Report bugs or issues via the project repository.

---

## Appendix A: Example Session

Let's point to Canopus (brightest southern star):

**Goal:** Canopus at culmination: ~78° altitude, 180° azimuth

**Session:**
```
[Power on TEC-1, execute telescope.bin]

Press: AD
Display: "ALT   "
(Selecting altitude axis)

Press: +
Display: "PLUS  "
(Moving upward)

Press: 7 8
Display: "    78"
(78 degrees)

Press: GO
Display: "    78"
(Confirmed, waiting for minutes)

Press: GO
Display: (counting down)
Motor: Moves to 78° altitude
(Double-GO executed with 0' 0")

[Wait for movement to complete]

Press: AD
Display: "ALT   "

Press: AD
Display: "AZZ   "
(Toggled to azimuth)

Press: +
Display: "PLUS  "

Press: 1 8 0
Display: "   180"
(South = 180°)

Press: GO GO
Display: (counting down)
Motor: Moves to 180° azimuth

[Telescope now pointing to Canopus position]
```

**Fine tuning:**
Use F mode to nudge:
```
F → AD → + → 50 GO → 1 GO    (50 steps up)
```

**Result:** Canopus should be in field of view!

---

## Appendix B: Conversion Formulas

### Degrees/Minutes/Seconds to Steps

```
Total arc-seconds = (deg × 3600) + (min × 60) + sec
Steps = arc-seconds × 2000 / 3600
      = arc-seconds × 5 / 9
```

**Example:** 45° 30' 15"
```
Arc-sec = (45 × 3600) + (30 × 60) + 15
        = 162000 + 1800 + 15
        = 163815 arc-seconds

Steps = 163815 × 5 / 9
      = 819075 / 9
      ≈ 91008 steps
```

### Steps to Degrees

```
Degrees = steps × 0.18
        = steps / 5.556
```

**Example:** 250 steps
```
Degrees = 250 / 5.556
        = 45°
```

### Arc-minutes per Step

```
0.18° per step = 10.8 arc-minutes per step
```

For precise positioning:
- 1 step ≈ 0.18° ≈ 11 arc-minutes
- 6 steps ≈ 1° ≈ 60 arc-minutes

---

## Appendix C: Maintenance

### Regular Checks

**Before each use:**
- [ ] Check motor wiring secure
- [ ] Verify telescope moves freely
- [ ] Test emergency stop (E key)
- [ ] Calibrate if moved

**Monthly:**
- [ ] Clean gear teeth
- [ ] Lubricate gears (light oil)
- [ ] Check motor mounting bolts
- [ ] Inspect wiring for wear

**Annually:**
- [ ] Check motor bearings
- [ ] Replace worn gears if needed
- [ ] Recalibrate fully
- [ ] Test all movement ranges

### Cleaning

**Motors:** Wipe with dry cloth, no solvents

**Drivers:** Blow out dust, check heat sinks

**Gears:** Light brush, regrease if needed

**TEC-1:** Standard electronics cleaning

### Storage

1. Power off completely
2. Cover motors from dust
3. Store in dry location
4. Disconnect batteries if unused > 1 month

---

**End of User Manual**

For technical details, see the other documentation files.

For support, refer to the project repository.

**Version:** 1.1
**Date:** 2026-01-19
**Status:** Production Ready ✅
