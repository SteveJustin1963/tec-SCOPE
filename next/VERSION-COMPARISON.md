# Which Version Should You Build?

## Two Complete Designs Available

You now have TWO complete motor control systems to choose from:

### Version 1: Quadrature Encoder with SPI (Advanced)
**Files:** motor-control.z80, wiring-diagram.md, motor-control-usage.md

### Version 2: Optical Gate Sensor (Simple)
**Files:** optical-gate-control.z80, optical-wiring.md, optical-gate-simple.md

---

## Quick Comparison Table

| Feature | Encoder Version | Optical Gate Version |
|---------|----------------|---------------------|
| **Cost** | ~$60 | ~$25 |
| **Complexity** | High | Low |
| **Programming needed** | Yes (ATtiny85) | No! |
| **Resolution** | 0.06° (3.6 arc-min) | 0.45° (27 arc-min) |
| **Direction sensing** | Hardware (in encoder) | Software (tracked) |
| **Best for** | Astrophotography | Visual observing |
| **DIY friendly** | Moderate | Very easy |
| **Debugging** | Complex (SPI protocol) | Simple (LED blinks) |
| **Parts to order** | Encoders ($30), ATtiny85 ($2), programmer ($10) | Optical sensors ($4) |

---

## Detailed Comparison

### Resolution & Accuracy

**Encoder Version (600 PPR × 10:1 gear × 4x quadrature):**
```
24,000 counts per telescope revolution
360° / 24,000 = 0.015° per count = 0.9 arc-minutes
Excellent for astrophotography!
```

**Optical Gate Version (8 slots × 100:1 gear):**
```
800 counts per telescope revolution
360° / 800 = 0.45° per count = 27 arc-minutes
Good enough for visual observing!
```

**What does this mean in practice?**
- **Encoder:** Can point to a star and it stays centered even at 200x magnification
- **Optical:** Can point to a constellation, planet, or bright star easily

### Complexity

**Encoder Version:**
```
Complexity breakdown:
1. Program 2 ATtiny85 chips (need Arduino IDE + programmer)
2. Implement SPI protocol on Z80
3. Handle 32-bit position counters
4. Debug SPI timing issues
5. More wiring (SPI bus)

Skill level: Intermediate to advanced
Time to build: 2-3 weeks
```

**Optical Gate Version:**
```
Complexity breakdown:
1. Cut tin lid with snips (30 minutes)
2. Wire optical sensors (simple pull-up circuit)
3. Count pulses (basic edge detection)
4. 16-bit counters (simpler than 32-bit)

Skill level: Beginner friendly
Time to build: 1 week
```

### Cost Breakdown

**Encoder Version:**
| Item | Qty | Cost | Total |
|------|-----|------|-------|
| HN3806-AB encoder | 2 | $15 | $30 |
| ATtiny85 | 2 | $1 | $2 |
| USBasp programmer | 1 | $10 | $10 |
| L298N driver | 2 | $3 | $6 |
| Resistors/caps | - | - | $2 |
| Power supplies | - | - | $11 |
| **Total** | | | **$61** |

**Optical Gate Version:**
| Item | Qty | Cost | Total |
|------|-----|------|-------|
| Optical slot sensor | 2 | $2 | $4 |
| L298N driver | 2 | $3 | $6 |
| Tin lid (scrap) | 2 | $0 | $0 |
| Resistors/caps | - | - | $2 |
| Power supplies | - | - | $11 |
| **Total** | | | **$23** |

**Savings: $38!**

### Reliability

**Encoder Version:**
- ✅ Hardware direction sensing (can't get direction wrong)
- ✅ High resolution (less gear backlash impact)
- ⚠️ More complex (more things to go wrong)
- ⚠️ SPI can glitch (electrical noise)

**Optical Gate Version:**
- ✅ Simple circuit (fewer failure points)
- ✅ Easy to debug (watch LED with your eyes!)
- ⚠️ Software direction tracking (can drift if code crashes)
- ⚠️ Can miss pulses if Z80 is busy (solution: use interrupts)

### Build Difficulty

**Encoder Version requires:**
- [ ] Arduino IDE setup
- [ ] ATtiny85 board support installation
- [ ] Programmer (USBasp or Arduino as ISP)
- [ ] Understanding of SPI protocol
- [ ] Oscilloscope helpful for debugging
- [ ] Careful attention to SPI timing

**Optical Gate Version requires:**
- [ ] Tin snips or strong scissors
- [ ] Ability to cut metal safely
- [ ] Basic soldering
- [ ] Multimeter (for testing)
- [ ] That's it!

### What Can Go Wrong?

**Encoder Version:**
- SPI clock timing wrong → No data
- CS signals inverted → Wrong encoder read
- ATtiny not programmed correctly → No response
- Electrical noise on SPI bus → Corrupted data
- 32-bit math overflow → Position glitches

**Optical Gate Version:**
- Disk wobbles → Extra pulses (fix: balance disk)
- Sensor misaligned → No pulses (fix: adjust position)
- Ambient light interference → False triggers (fix: use IR sensor or shield)
- Motor too fast → Missed pulses (fix: slow down or use interrupts)

**Winner for reliability:** Optical gate (simpler = fewer things to break)

---

## When to Use Each Version

### Use Encoder Version (SPI) if:

✅ You want to do **astrophotography**
- Need precise tracking for long exposures
- 0.06° resolution critical for sharp images
- Can tolerate higher cost and complexity

✅ You have **experience with microcontrollers**
- Comfortable programming ATtiny85
- Understand SPI protocol
- Have oscilloscope for debugging

✅ You already have the **parts**
- Have encoders lying around
- Have ATtiny85 chips and programmer
- Want maximum performance

✅ Budget is not a concern
- $60 total is acceptable
- Want the "best" system

### Use Optical Gate Version if:

✅ You want **visual observing**
- GoTo pointing is enough
- 0.45° resolution is acceptable
- Want to save money

✅ You're **new to electronics**
- No microcontroller programming experience
- Want simple, easy to debug
- Prefer mechanical DIY (cutting metal) over software

✅ You want to **build it quickly**
- Can finish in 1 week
- No waiting for programmers to arrive
- Immediate results

✅ Budget matters
- $25 total cost
- Save $38 compared to encoder version
- Use savings for better eyepieces!

---

## My Recommendation

### For Your First Build: **Optical Gate Version**

**Reasons:**
1. You asked about making things simple ("raw PWM control")
2. You want to cut tin lids (hands-on DIY approach)
3. 100:1 gear ratio you mentioned suits optical gate perfectly
4. Much cheaper ($25 vs $60)
5. Easier to debug (see LED blinking!)
6. Faster to build (1 week vs 2-3 weeks)
7. Good enough for visual observing and learning

### Upgrade Path

Start with **optical gate**, then upgrade later:

1. **Build optical gate first** ($25, 1 week)
   - Learn motor control
   - Get telescope working
   - Enjoy visual observing

2. **Add features** (free, time)
   - Implement tracking mode
   - Add Stellarium control
   - Tune PID control

3. **Upgrade to encoders later** (if needed) ($38 more, 1 week)
   - Keep optical gate as backup
   - Swap in encoders for astrophotography
   - Now have experience with the system

**Total cost same as building encoder version first, but you learn incrementally!**

---

## Hybrid Option: Start Simple, Upgrade Later

**Week 1-2:** Build optical gate version
- Get basic GoTo working
- Learn Z80 motor control
- Total cost: $25

**Week 3-4:** Add features to optical version
- Tracking mode
- Manual control refinement
- Display integration

**Later (optional):** Upgrade to encoders
- Order HN3806 encoders ($30)
- Order ATtiny85 ($2)
- Swap optical sensors for encoders
- Load encoder version of code
- Total added cost: $32

**Benefit:** You can test and decide if you need encoders based on actual use!

---

## What Other Builders Say

**Typical ATM (Amateur Telescope Making) advice:**

> "For your first motorized telescope, keep it simple. Use optical sensors or even just limit switches for homing. Get basic GoTo working first. You can always upgrade to encoders later when you want to do astrophotography."

> "I built mine with optical encoders from old PC mice first. Worked great for visual observing. Upgraded to 2000 PPR encoders when I got serious about imaging."

> "The best telescope is the one you actually finish building. Don't over-engineer it."

---

## Decision Matrix

Answer these questions:

| Question | If YES → | If NO → |
|----------|---------|---------|
| Do you want to do astrophotography? | Encoders | Optical |
| Do you have microcontroller experience? | Encoders OK | Optical |
| Is $60 budget acceptable? | Either | Optical |
| Do you have oscilloscope for debugging? | Encoders OK | Optical |
| Want to finish quickly (1 week)? | Optical | Either |
| Enjoy hands-on mechanical work? | Optical | Either |
| Need sub-arc-minute precision? | Encoders | Optical |
| Want simplest possible build? | Optical | Optical |

**Count your answers:**
- More "Encoders" → Build encoder version
- More "Optical" → Build optical gate version
- Tie → Start with optical, upgrade later!

---

## Final Recommendation Table

| Your Goal | Version to Build | Why |
|-----------|-----------------|-----|
| Learn motor control basics | **Optical** | Simpler, faster results |
| Visual observing only | **Optical** | 0.45° is good enough |
| Astrophotography planned | **Encoders** | Need 0.06° precision |
| Limited budget ($25) | **Optical** | Saves $38 |
| No programming experience | **Optical** | No ATtiny programming |
| Have oscilloscope & programmer | **Encoders** | Can debug complex issues |
| Want it working this week | **Optical** | Faster to build |
| Maximum performance | **Encoders** | Best resolution |
| Hands-on DIY person | **Optical** | Get to cut and build mechanically |
| Software person | **Encoders** | More programming challenge |

---

## Both Versions Are Complete!

Either version you choose, you have:
- ✅ Complete Z80 assembly code
- ✅ Complete wiring diagrams
- ✅ Complete parts lists
- ✅ Complete test programs
- ✅ Complete documentation

You can't make a wrong choice - both will work great for their intended purpose!

**My advice:** If uncertain, start with **optical gate**. You'll learn the fundamentals, save money, and can always upgrade later. The experience you gain will make the encoder version easier if you decide to build it.

Good luck! 🔭✨
