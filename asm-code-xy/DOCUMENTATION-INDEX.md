# Documentation Index

**Quick guide to finding the right documentation for your needs**

---

## 👤 I Want To...

### ...Use the Telescope

**Start here:** [USER-MANUAL.md](USER-MANUAL.md)
- Complete step-by-step guide
- Keypad controls explained
- Common tasks and examples
- Troubleshooting

**Quick reference:** [QUICK-REFERENCE.md](QUICK-REFERENCE.md)
- Command cheat sheet
- Key functions
- Conversion tables
- Memory map

### ...Build the Hardware

**Start here:** [HARDWARE.md](HARDWARE.md)
- Complete wiring diagrams
- L298N connections
- TEC-1 port assignments
- Component list
- Testing procedures

**Specifications:** [tec1-sbc-spec.md](tec1-sbc-spec.md)
- TEC-1 hardware details
- Port mapping
- Display encoding
- Keyboard interface

### ...Understand the Keypad

**Start here:** [KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md)
- Complete keypad guide
- Input modes explained (AD and F)
- State machine diagrams
- Display feedback
- Error handling

**Quick reference:** [USER-MANUAL.md > Keypad Controls](USER-MANUAL.md#keypad-controls)

### ...Calibrate for Sydney

**Start here:** [USER-MANUAL.md > Calibration](USER-MANUAL.md#calibration)
- Simple 3-point calibration
- Sydney-specific notes
- Reference stars

**Technical details:** [SYSTEM-OVERVIEW.md > Calibration](SYSTEM-OVERVIEW.md#calibration-procedure-sydney-australia)

### ...Fix Problems

**Start here:** [USER-MANUAL.md > Troubleshooting](USER-MANUAL.md#troubleshooting)
- Common problems and solutions
- Motors don't move
- Display errors
- Wrong direction
- Position drift

**Recent fixes:** [BUG-FIXES.md](BUG-FIXES.md)
- Bugs fixed in v1.1
- SLEW early exit
- Limit checking
- Stepping mode upgrade

### ...Change Motor Drive Mode

**Start here:** [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md)
- Wave drive vs Full-step vs Half-step
- Torque comparison
- How to switch modes
- Microstepping upgrade path

**Current config:** Full-step mode (0011b, 0110b, 1100b, 1001b)

---

## 👨‍💻 Developer Documentation

### Code Architecture

**Overview:** [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md)
- Complete system architecture
- Memory map
- I/O ports
- Data flow diagrams
- Module descriptions

**Flow charts:** [flow.md](flow.md)
- ASCII flowcharts for all programs
- State machines
- Execution paths

### Source Code Files

**Configuration:**
- [config.z80](config.z80) - Constants, step patterns, memory locations

**Core Library:**
- [stepper-lib.z80](stepper-lib.z80) - Motor control functions
- [keypad-input.z80](keypad-input.z80) - TEC-1 keyboard interface

**Main Programs:**
- [main.z80](main.z80) - Main program with calibration
- [simple-test.z80](simple-test.z80) - Standalone motor test

### Implementation Status

**Current status:** [IMPLEMENTATION-STATUS.md](IMPLEMENTATION-STATUS.md)
- What's implemented (everything!)
- Bugs fixed
- Known limitations
- Memory usage
- Testing checklist

**Recent changes:** [CHANGES-SUMMARY.md](CHANGES-SUMMARY.md)
- Changes in v1.1
- Before/after comparison
- Files modified

**Bug fixes:** [BUG-FIXES.md](BUG-FIXES.md)
- Critical bug: SLEW early exit
- Limit checking added
- Stepping mode upgraded
- Testing procedures

### Stepping Modes

**Complete guide:** [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md)
- Wave drive (original, low torque)
- Full-step (current, recommended)
- Half-step (smooth, requires code changes)
- Microstepping (hardware upgrade)
- Physics explanations
- How to switch

---

## 📊 Quick Reference by Topic

### Hardware

| Topic | Document |
|-------|----------|
| Wiring diagrams | HARDWARE.md |
| L298N connections | HARDWARE.md > L298N Module Connections |
| TEC-1 port map | tec1-sbc-spec.md > I/O Port Map |
| Component list | HARDWARE.md > Component List |
| Power supply | HARDWARE.md > Power Supply Design |
| Testing | HARDWARE.md > Testing Procedure |

### Software

| Topic | Document |
|-------|----------|
| User guide | USER-MANUAL.md |
| Quick reference | QUICK-REFERENCE.md |
| Keypad controls | KEYPAD-INTERFACE.md |
| Code architecture | SYSTEM-OVERVIEW.md |
| Function reference | stepper-lib.z80 (commented) |
| Flowcharts | flow.md |

### Operation

| Topic | Document |
|-------|----------|
| Getting started | USER-MANUAL.md > Quick Start |
| First time setup | USER-MANUAL.md > First Time Setup |
| Common tasks | USER-MANUAL.md > Common Tasks |
| Keypad modes | USER-MANUAL.md > Basic Operations |
| Calibration | USER-MANUAL.md > Calibration |
| Troubleshooting | USER-MANUAL.md > Troubleshooting |

### Configuration

| Topic | Document |
|-------|----------|
| Stepping modes | STEPPING-OPTIONS.md |
| Speed settings | config.z80 (SLEW_DELAY, etc.) |
| Step limits | config.z80 (ALT_MAX, AZ_MAX) |
| Sydney calibration | config.z80 (SCP_*, CAL_*) |
| Port assignments | config.z80 (ALT_PORT, AZ_PORT) |

### Advanced

| Topic | Document |
|-------|----------|
| Half-stepping | STEPPING-OPTIONS.md > Half-Stepping |
| Microstepping | STEPPING-OPTIONS.md > Microstepping |
| Code modifications | stepper-lib.z80, main.z80 |
| Calibration API | main.z80 (CALIBRATE_* functions) |
| Sidereal tracking | main.z80 (START_SIDEREAL_TRACKING) |

---

## 📖 Documentation by Experience Level

### Beginner (Never Used Before)

**Read in this order:**
1. [README.md](README.md) - Project overview
2. [USER-MANUAL.md](USER-MANUAL.md) - Complete usage guide
3. [QUICK-REFERENCE.md](QUICK-REFERENCE.md) - Keep handy

**When building:**
- [HARDWARE.md](HARDWARE.md) - Follow step-by-step

### Intermediate (Used Similar Systems)

**Essential reading:**
1. [QUICK-REFERENCE.md](QUICK-REFERENCE.md) - Quick start
2. [KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md) - Understand input modes
3. [USER-MANUAL.md > Calibration](USER-MANUAL.md#calibration) - Sydney setup

**Reference:**
- [USER-MANUAL.md](USER-MANUAL.md) - When needed

### Advanced (Modifying Code)

**Start here:**
1. [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md) - Architecture
2. [flow.md](flow.md) - Program flow
3. [IMPLEMENTATION-STATUS.md](IMPLEMENTATION-STATUS.md) - What's done

**Source code:**
- [config.z80](config.z80) - Modify constants
- [stepper-lib.z80](stepper-lib.z80) - Motor functions
- [main.z80](main.z80) - Main program

**Optimization:**
- [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md) - Drive modes
- [BUG-FIXES.md](BUG-FIXES.md) - Recent changes

---

## 🔍 Documentation by File Type

### User Guides

- [USER-MANUAL.md](USER-MANUAL.md) - Complete user manual
- [QUICK-REFERENCE.md](QUICK-REFERENCE.md) - Command reference card
- [KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md) - Keypad system guide

### Hardware Guides

- [HARDWARE.md](HARDWARE.md) - Wiring and connections
- [tec1-sbc-spec.md](tec1-sbc-spec.md) - TEC-1 specifications

### System Documentation

- [README.md](README.md) - Project overview
- [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md) - Architecture overview
- [flow.md](flow.md) - Program flowcharts

### Developer Documentation

- [IMPLEMENTATION-STATUS.md](IMPLEMENTATION-STATUS.md) - Status tracking
- [BUG-FIXES.md](BUG-FIXES.md) - Bug fix details
- [CHANGES-SUMMARY.md](CHANGES-SUMMARY.md) - Recent changes
- [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md) - Motor drive modes

### Source Code

- [config.z80](config.z80) - Configuration constants
- [stepper-lib.z80](stepper-lib.z80) - Motor control library
- [keypad-input.z80](keypad-input.z80) - Keyboard interface
- [main.z80](main.z80) - Main program
- [simple-test.z80](simple-test.z80) - Test program

### Build System

- [Makefile](Makefile) - Build automation

---

## 📝 What's New in Version 1.1

**Date:** 2026-01-19

**Critical fixes applied:**
- ✅ SLEW_TO_POSITION early exit bug fixed (both axes complete)
- ✅ Limit checking added (prevents hardware damage)
- ✅ Upgraded to full-step drive (41% more torque)

**New documentation:**
- ✅ USER-MANUAL.md - Complete user guide
- ✅ BUG-FIXES.md - Detailed fix documentation
- ✅ STEPPING-OPTIONS.md - Motor drive comparison
- ✅ CHANGES-SUMMARY.md - Quick change reference
- ✅ DOCUMENTATION-INDEX.md - This file

**See:** [CHANGES-SUMMARY.md](CHANGES-SUMMARY.md) for details

---

## 🆘 Need Help?

### "I just want to use it"
→ [USER-MANUAL.md](USER-MANUAL.md)

### "Motors don't work"
→ [USER-MANUAL.md > Troubleshooting](USER-MANUAL.md#troubleshooting)
→ [HARDWARE.md > Testing Procedure](HARDWARE.md#testing-procedure)

### "How do I enter coordinates?"
→ [USER-MANUAL.md > Basic Operations](USER-MANUAL.md#basic-operations)
→ [KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md)

### "What do these keys do?"
→ [USER-MANUAL.md > Keypad Controls](USER-MANUAL.md#keypad-controls)
→ [QUICK-REFERENCE.md](QUICK-REFERENCE.md)

### "How do I calibrate?"
→ [USER-MANUAL.md > Calibration](USER-MANUAL.md#calibration)

### "Can I make it faster/smoother?"
→ [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md)

### "I want to modify the code"
→ [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md)
→ [flow.md](flow.md)
→ Source files in repository

### "What changed recently?"
→ [CHANGES-SUMMARY.md](CHANGES-SUMMARY.md)
→ [BUG-FIXES.md](BUG-FIXES.md)

---

## 📚 Reading List by Goal

### Goal: Get telescope working

1. [HARDWARE.md](HARDWARE.md) - Build hardware
2. [USER-MANUAL.md > First Time Setup](USER-MANUAL.md#first-time-setup)
3. [USER-MANUAL.md > Basic Operations](USER-MANUAL.md#basic-operations)

### Goal: Point to specific stars

1. [USER-MANUAL.md > Calibration](USER-MANUAL.md#calibration)
2. [USER-MANUAL.md > Common Tasks](USER-MANUAL.md#common-tasks)
3. [KEYPAD-INTERFACE.md](KEYPAD-INTERFACE.md)

### Goal: Understand the system

1. [README.md](README.md)
2. [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md)
3. [flow.md](flow.md)

### Goal: Modify performance

1. [STEPPING-OPTIONS.md](STEPPING-OPTIONS.md)
2. [config.z80](config.z80)
3. [stepper-lib.z80](stepper-lib.z80)

### Goal: Add features

1. [SYSTEM-OVERVIEW.md](SYSTEM-OVERVIEW.md)
2. [IMPLEMENTATION-STATUS.md](IMPLEMENTATION-STATUS.md)
3. [main.z80](main.z80), [stepper-lib.z80](stepper-lib.z80)

---

## 📄 Complete File List

### Documentation (Markdown)

| File | Size | Purpose |
|------|------|---------|
| README.md | Large | Project overview |
| USER-MANUAL.md | **Very Large** | **Complete user guide** ⭐ |
| QUICK-REFERENCE.md | Medium | Command reference |
| HARDWARE.md | Large | Hardware guide |
| KEYPAD-INTERFACE.md | Very Large | Keypad system |
| SYSTEM-OVERVIEW.md | Large | Architecture |
| IMPLEMENTATION-STATUS.md | Large | Status tracking |
| tec1-sbc-spec.md | Medium | TEC-1 spec |
| flow.md | Large | Flowcharts |
| BUG-FIXES.md | Medium | Bug fixes (v1.1) |
| STEPPING-OPTIONS.md | Very Large | Motor modes |
| CHANGES-SUMMARY.md | Medium | Changes (v1.1) |
| DOCUMENTATION-INDEX.md | Medium | This file |

### Source Code (Z80 Assembly)

| File | Lines | Purpose |
|------|-------|---------|
| config.z80 | ~300 | Configuration |
| stepper-lib.z80 | ~450 | Motor library |
| keypad-input.z80 | ~1350 | Keyboard interface |
| main.z80 | ~675 | Main program |
| simple-test.z80 | ~150 | Test program |

### Build System

| File | Purpose |
|------|---------|
| Makefile | Build automation |

---

**Last Updated:** 2026-01-19
**Version:** 1.1
**Status:** Production Ready ✅

**Start here:** [USER-MANUAL.md](USER-MANUAL.md) 📘
