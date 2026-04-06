# TEC-1 Keypad Interface Guide

## Overview

The telescope control system uses the TEC-1's 4×5 keypad (74C923 encoder) for all user input. Two distinct input modes provide both precision positioning and fast manual control.

## Hardware Interface

**74C923 Keyboard Encoder:**
- Triggers **NMI interrupt** on keypress
- Read 5-bit key code from **Port 00h** (D0-D4)
- Key codes: 0x00-0x0F (hex digits), 0x10-0x13 (function keys)

**TEC-1 Display:**
- 6-digit 7-segment display
- Digit select: **Port 01h** (6 bits, one per digit)
- Segment data: **Port 02h** (8 bits for segments a-g + dp)

## Key Mapping

| Key | Code | Function |
|-----|------|----------|
| 0-9 | 00h-09h | Digit entry |
| A-F | 0Ah-0Fh | Hex digits (C, E, F used) |
| + | 10h | Positive direction |
| - | 11h | Negative direction |
| GO | 12h | Confirm / Execute |
| AD | 13h | Toggle ALT/AZZ or Abort |
| C | 0Ch | Skip field (null input) |
| E | 0Eh | **RS (Reset)** - Halt & abort |
| F | 0Fh | Enter fast mode |

**Note:** Key **E** is used as **RS (Reset)** key

## Input Modes

### MODE 1: Precision (Deg/Min/Sec)

Enter celestial coordinates in degrees, arc-minutes, and arc-seconds.

**Entry Flow:**
```
AD          → Toggle "ALT" / "AZZ" display
+/- (req'd) → Direction: "PLUS" or "NEG"
DD GO       → Degrees (0-360)
MM GO       → Minutes (0-59)
SS GO       → Seconds (0-59)
            → Executes movement
```

**Examples:**

**Set Altitude +45° 30' 15":**
```
AD → "ALT   "
+  → "PLUS  "
45 GO → "    45"
30 GO → "    30"
15 GO → "    15" → Executes
```

**Set Azimuth -120° 0' 0" (quick entry with double-GO):**
```
AD → "ALT   "
AD → "AZZ   " (toggle)
-  → "NEG   "
120 GO → "   120"
GO → Executes (double-GO shortcut)
```

**Skip fields with C:**
```
AD → "ALT   "
+  → "PLUS  "
C  → (skip degrees = 0)
33 GO → "    33" (33 arc-minutes)
GO → Executes +0° 33' 0"
```

### MODE 2: Fast Step Count

Direct step count entry with variable speed (1-9x multiplier).

**Entry Flow:**
```
F           → "FAST  " display
AD          → Toggle "ALT" / "AZZ"
+/- (req'd) → Direction
SSSS GO     → Step count (1-65535)
M GO        → Speed multiplier (1-9)
            → Executes fast movement
```

**Examples:**

**Move Altitude +10000 steps at 4x speed:**
```
F     → "FAST  "
AD    → "ALT   "
+     → "PLUS  "
10000 GO → " 10000"
4 GO  → Executes at 4x speed
```

**Move Azimuth -5000 steps at normal speed:**
```
F    → "FAST  "
AD   → "ALT   "
AD   → "AZZ   " (toggle)
-    → "NEG   "
5000 GO → "  5000"
1 GO → Executes at 1x (normal) speed
```

**Quick entry (defaults to speed 1):**
```
F → AD → + → 1000 GO GO
(Double-GO uses default speed)
```

## Control Keys

### AD Key Behavior

**When Idle:**
- Toggles between "ALT" and "AZZ" display
- Sets mode to AD (deg/min/sec) mode
- Waits for +/- input

**During F Mode:**
- After pressing F, AD toggles ALT/AZZ
- Stays in F mode

### RS Key (E key) - Reset/Abort

**Emergency Stop:**
- Immediately halts both motors (outputs 0000b)
- Clears motor state flags
- **Preserves current position** in memory
- Returns to idle state

**Use Cases:**
- Stop mid-slew if wrong target entered
- Emergency halt for safety
- Abort incorrect entry and start over

**Position Preservation:**
- ALT_POS_LO/HI and AZ_POS_LO/HI **NOT** zeroed
- Next movement starts from current position
- Allows resumption after abort

### GO Key

**Single Press:**
- Confirms current field
- Advances to next field (deg→min→sec or steps→speed)
- Clears GO counter

**Double Press (GO GO):**
- Executes immediately with current values
- Remaining fields default to 0
- Quick shortcut for partial entry

### C Key - Skip Field

**Single C:**
- Skips current field (enters 0)
- Advances to next field

**Examples:**
```
C C 22 GO → Skip deg, skip min, enter 22 sec
C 30 GO GO → Skip deg, enter 30 min
```

### +/- Keys - Direction (MANDATORY)

**Must Always Be Pressed:**
- If omitted, displays "ERR  " for 2 seconds
- Returns to start (prevents accidental movement)

**Direction Meanings:**
- **Altitude:** + = up (increasing), - = down (decreasing)
- **Azimuth:** + = CW/East, - = CCW/West

## Display Feedback

### Text Displays

| Display | Meaning |
|---------|---------|
| ALT    | Altitude axis selected |
| AZZ    | Azimuth axis selected |
| PLUS   | Positive direction |
| NEG    | Negative direction |
| FAST   | Fast mode active |
| ERR    | Error (missing +/- sign) |

### Number Displays

**During Entry:**
- Shows digits as typed
- Right-justified on 6-digit display
- Example: typing "123" shows "   123"

**During Movement:**
- Shows remaining step count
- Counts down to 0
- Provides visual feedback of progress

## State Machine

```
┌─────────────────────────────────────────────────────────┐
│                      IDLE STATE                         │
│  - Display blank or last position                       │
│  - Waiting for AD or F key                              │
└────────────┬───────────────────────────┬────────────────┘
             │                           │
        ┌────▼────┐                 ┌────▼────┐
        │ AD Key  │                 │  F Key  │
        └────┬────┘                 └────┬────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │   AD MODE       │         │   F MODE        │
    │  (Deg/Min/Sec)  │         │  (Fast Steps)   │
    └────────┬────────┘         └────────┬────────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │ Toggle ALT/AZZ  │         │   Show "FAST"   │
    │ Display selected│         │   Wait for AD   │
    └────────┬────────┘         └────────┬────────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │  Wait for +/-   │         │ Toggle ALT/AZZ  │
    │  (MANDATORY)    │         │  Wait for +/-   │
    └────────┬────────┘         └────────┬────────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │ Show "PLUS" or  │         │ Show "PLUS" or  │
    │ "NEG" on display│         │ "NEG" on display│
    └────────┬────────┘         └────────┬────────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │ Enter Degrees   │         │ Enter Steps     │
    │ C = skip, GO = next       │ C = skip, GO = next       │
    └────────┬────────┘         └────────┬────────┘
             │                           │
    ┌────────▼────────┐         ┌────────▼────────┐
    │ Enter Minutes   │         │ Enter Speed     │
    │ C = skip, GO = next       │ 1-9 only        │
    └────────┬────────┘         │ 0 = ignored     │
             │                  └────────┬────────┘
    ┌────────▼────────┐                 │
    │ Enter Seconds   │                 │
    │ C = skip, GO = exec              │
    └────────┬────────┘                 │
             │                           │
             └──────────┬────────────────┘
                        │
              ┌─────────▼─────────┐
              │ EXECUTE MOVEMENT  │
              │ - Convert to steps│
              │ - Apply direction │
              │ - Display countdown│
              │ - Move motors     │
              └─────────┬─────────┘
                        │
              ┌─────────▼─────────┐
              │  Return to IDLE   │
              │  - Clear buffers  │
              │  - Ready for next │
              └───────────────────┘

          ┌──────────────────────┐
          │   ABORT at anytime:  │
          │   RS Key (E)         │
          │   → Halt motors      │
          │   → Preserve position│
          │   → Return to IDLE   │
          └──────────────────────┘
```

## Conversion Formulas

### Deg/Min/Sec to Steps

```
Total arc-seconds = (deg × 3600) + (min × 60) + sec
Steps = arc-seconds × 2000 / 3600
      = arc-seconds × 5 / 9
```

**Example:** 45° 30' 15" = ?
```
Arc-seconds = (45 × 3600) + (30 × 60) + 15
            = 162000 + 1800 + 15
            = 163815 arc-seconds

Steps = 163815 × 5 / 9
      = 819075 / 9
      = 91008 steps (approximately)
```

### Speed Multiplier

Speed multiplier divides the step delay:
```
Adjusted delay = SLEW_DELAY / speed_multiplier

Speed 1: SLEW_DELAY / 1 = 50 (normal)
Speed 2: SLEW_DELAY / 2 = 25 (2x faster)
Speed 4: SLEW_DELAY / 4 = 12.5 (4x faster)
Speed 9: SLEW_DELAY / 9 = 5.5 (9x faster)
```

**Warning:** High speeds (8-9x) may cause:
- Missed steps if motor torque insufficient
- Loss of position accuracy
- Mechanical stress

**Recommended:**
- Speed 1-2: Normal operation
- Speed 3-5: Fast slewing
- Speed 6-9: Light loads only, test carefully

## Usage Examples

### Example 1: Point to Polaris (from Sydney)

Sydney latitude = 34°S, so SCP altitude ≈ 34°
Polaris not visible from Southern Hemisphere, but for demonstration:

```
Keypress:       Display:
AD              "ALT   "
+               "PLUS  "
3 4 GO          "    34"
0 GO            "     0"
0 GO            "     0" → Executes to 34° 0' 0"

AD              "ALT   "
AD              "AZZ   "
+               "PLUS  "
1 8 0 GO        "   180"
GO              Executes to 180° (south)
```

### Example 2: Quick Offset Correction

Need to move +5 arc-minutes in altitude:

```
AD              "ALT   "
+               "PLUS  "
C               (skip degrees)
5 GO            "     5"
GO              Executes +0° 5' 0"
```

### Example 3: Fast Manual Positioning

Rough positioning with 5000 steps at 3x speed:

```
F               "FAST  "
AD              "AZZ   "
-               "NEG   "
5 0 0 0 GO      "  5000"
3 GO            Executes -5000 steps at 3x speed
```

### Example 4: Emergency Stop

Started wrong slew, need to abort:

```
(Movement in progress, display counting down)
E               → Motors HALT immediately
                → Position preserved
                → Returns to idle

(Can now enter correct target)
```

## Error Handling

### Error Conditions

**Missing +/- Sign:**
```
AD → 45 GO GO
→ Display "ERR  " for 2 seconds
→ Return to idle
```

**Prevention:** Always press + or - after AD/AZZ selection

**Speed = 0:**
```
In F mode: 1000 GO → 0 GO
→ 0 is ignored, waits for 1-9
```

**Invalid Step Count:**
```
Entry > 65535 in F mode
→ 16-bit overflow (wraps around)
→ Limit input to 6 digits max
```

### Recovery Procedures

**Stuck in Entry Mode:**
- Press **RS (E key)** to abort
- Returns to idle immediately

**Wrong Axis Selected:**
- Before pressing +/-, press AD to toggle
- After +/- pressed, must use RS to abort

**Wrong Direction:**
- Must use RS to abort, then re-enter
- Cannot change direction mid-entry

**Display Frozen:**
- Press RS to reset display
- May need hardware reset if unresponsive

## Tips & Best Practices

### Accuracy Tips

1. **Use AD mode for precision:**
   - Celestial coordinates are in deg/min/sec
   - Directly matches star charts

2. **Use F mode for rough positioning:**
   - Quick setup movements
   - Bring target into view before fine adjustment

3. **Leverage double-GO:**
   - `45 GO GO` = 45° 0' 0" (fast)
   - Useful for whole-degree movements

4. **Skip fields with C:**
   - Faster than entering zeros
   - `C C 30 GO` = 0° 0' 30"

### Speed Selection

**Recommended speeds by application:**
- **Speed 1**: Precision tracking, final positioning
- **Speed 2-3**: Normal slewing between targets
- **Speed 4-5**: Fast repositioning, daytime setup
- **Speed 6-9**: Only for unloaded testing

**Test before observing:**
```
F → AD → + → 1000 GO → 1 GO    (slow test)
F → AD → + → 1000 GO → 5 GO    (fast test)
```

### Position Tracking

**After RS (abort):**
- Position counters show true location
- Next movement is relative to current position
- Use known reference star to re-calibrate if unsure

**Check current position:**
- Read ALT_POS_LO (8000h-8001h)
- Read AZ_POS_LO (8002h-8003h)
- Can display on TEC-1 (future enhancement)

## Troubleshooting

| Problem | Cause | Solution |
|---------|-------|----------|
| Display shows "ERR  " | Missing +/- sign | Always press + or - after AD |
| Nothing happens after GO | Still in field entry | Press GO again (double-GO) |
| Wrong axis moving | Selected wrong axis | Check ALT vs AZZ before +/- |
| Movement too slow | Speed = 1 in F mode | Use higher speed (2-9) |
| Movement too fast | Speed too high | Lower speed or use AD mode |
| Can't abort entry | Unknown state | Press RS (E key) to reset |
| Motors don't stop | Software issue | Hardware power-off if needed |
| Display garbled | Interrupt timing | Press RS to reset display |

## Memory Map

**Input System Memory (8020h-802Fh):**
```
8020h - INPUT_MODE      (0=idle, 1=AD, 2=F)
8021h - INPUT_AXIS      (0=ALT, 1=AZZ)
8022h - INPUT_SIGN      (0=positive, 1=negative)
8023h - INPUT_STATE     (0-5, field state)
8024h - INPUT_DEG       (16-bit degrees/steps)
8026h - INPUT_MIN       (16-bit minutes)
8028h - INPUT_SEC       (16-bit seconds)
802Ah - INPUT_SPEED     (1-9 multiplier)
802Bh - INPUT_BUFFER    (16-bit accumulator)
802Dh - INPUT_GO_COUNT  (GO press counter)
802Eh - LAST_KEY        (Last key code read)
802Fh - TEMP_SPEED      (Temporary speed storage)
```

## Integration Notes

**For Developers:**

1. **NMI Handler Location:**
   - Must be at address 0066h
   - Handles all keyboard interrupts
   - Calls PROCESS_KEY dispatcher

2. **Display Multiplexing:**
   - Currently uses static display (one update per key)
   - Future: add display refresh in main loop
   - Prevents flicker on 6-digit display

3. **Interrupt Safety:**
   - Motor movement done outside interrupt
   - Long delays in EXECUTE_MOVEMENT
   - Consider disabling interrupts during movement

4. **Extension Points:**
   - Add position display in main loop
   - Implement goto-catalog (star database)
   - Add tracking mode toggle via keypad

## Future Enhancements

- [ ] Display current position continuously
- [ ] Named star catalog (press 'A' for Acrux, 'B' for Becrux, etc.)
- [ ] Sidereal tracking on/off via keypad
- [ ] Save/recall favorite positions
- [ ] Calibration wizard mode
- [ ] Help screen (cycle through key functions)

---

**See also:**
- `tec1-sbc-spec.md` - TEC-1 hardware details
- `config.z80` - System configuration constants
- `keypad-input.z80` - Implementation source code
