● Here are ASCII flowcharts for all the current programs:

  ┌─────────────────────────────────────────────────────────────────────────┐
  │                         1. SIMPLE-TEST.Z80                              │
  │                    (Simple Motor Test Program)                          │
  └─────────────────────────────────────────────────────────────────────────┘

      ┌─────────┐
      │  START  │
      └────┬────┘
           │
           ├─→ Initialize Stack (SP = 0FFFFH)
           │
           ├─→ ┌──────────────────────────┐
           │   │  TEST1_ALT_CW            │
           │   │  Step Alt motor CW       │
           │   │  100 steps               │
           │   └──────────────────────────┘
           │
           ├─→ Long Delay
           │
           ├─→ ┌──────────────────────────┐
           │   │  TEST2_ALT_CCW           │
           │   │  Step Alt motor CCW      │
           │   │  100 steps               │
           │   └──────────────────────────┘
           │
           ├─→ Long Delay
           │
           ├─→ ┌──────────────────────────┐
           │   │  TEST3_AZ_CW             │
           │   │  Step Az motor CW        │
           │   │  100 steps               │
           │   └──────────────────────────┘
           │
           ├─→ Long Delay
           │
           ├─→ ┌──────────────────────────┐
           │   │  TEST4_BOTH              │
           │   │  Step BOTH motors CW     │
           │   │  100 steps each          │
           │   └──────────────────────────┘
           │
           └─→ HALT

      ┌──────────────────────────────────────────────────┐
      │  Each TEST routine uses:                         │
      │  • Loop counter (B register)                     │
      │  • Step index (C/D registers)                    │
      │  • GET_STEP_PATTERN - lookup table               │
      │  • OUT to port 03h/04h                           │
      │  • SHORT_DELAY between steps                     │
      └──────────────────────────────────────────────────┘


  ┌─────────────────────────────────────────────────────────────────────────┐
  │                          2. MAIN.Z80                                    │
  │                    (Main Telescope Program)                             │
  └─────────────────────────────────────────────────────────────────────────┘

      ┌─────────┐
      │  START  │
      └────┬────┘
           │
           ├─→ Initialize Stack (SP = 0FFFFH)
           │
           ├─→ ┌─────────────────────┐
           │   │  INIT_MOTORS        │
           │   │  (from lib)         │
           │   │  • Zero positions   │
           │   │  • Set step idx = 0 │
           │   │  • Output STEP_0    │
           │   └─────────────────────┘
           │
           └─→ ┌─────────────────────┐
               │  MAIN_MENU          │
               └──────┬──────────────┘
                      │
                      ├─→ ┌────────────────────────┐
                      │   │  EXAMPLE_HOME          │
                      │   │  • HOME_TELESCOPE      │
                      │   │  • Delay 5000          │
                      │   └────────────────────────┘
                      │
                      ├─→ ┌────────────────────────┐
                      │   │  EXAMPLE_SLEW_1        │
                      │   │  Alt=250, Az=500       │
                      │   │  (45°, 90°)            │
                      │   │  • SET_TARGET_POSITION │
                      │   │  • SLEW_TO_POSITION    │
                      │   │  • Delay 5000          │
                      │   └────────────────────────┘
                      │
                      ├─→ ┌────────────────────────┐
                      │   │  EXAMPLE_SLEW_2        │
                      │   │  Alt=333, Az=1000      │
                      │   │  (60°, 180°)           │
                      │   │  • SET_TARGET_POSITION │
                      │   │  • SLEW_TO_POSITION    │
                      │   │  • Delay 5000          │
                      │   └────────────────────────┘
                      │
                      ├─→ ┌────────────────────────┐
                      │   │  EXAMPLE_HOME          │
                      │   │  Return to (0,0)       │
                      │   └────────────────────────┘
                      │
                      └─→ HALT


  ┌─────────────────────────────────────────────────────────────────────────┐
  │                       3. STEPPER-LIB.Z80                                │
  │                  (Stepper Motor Library Functions)                      │
  └─────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  INITIALIZATION                                                          │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────┐                                                    │
  │  │  INIT_MOTORS    │                                                    │
  │  └────┬────────────┘                                                    │
  │       ├─→ Zero ALT_POS_LO/HI                                            │
  │       ├─→ Zero AZ_POS_LO/HI                                             │
  │       ├─→ Zero ALT_TARGET_LO/HI                                         │
  │       ├─→ Zero AZ_TARGET_LO/HI                                          │
  │       ├─→ Set ALT_STEP_IDX = 0                                          │
  │       ├─→ Set AZ_STEP_IDX = 0                                           │
  │       ├─→ Clear MOTOR_STATE                                             │
  │       ├─→ OUT (ALT_PORT), STEP_0                                        │
  │       └─→ OUT (AZ_PORT), STEP_0                                         │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  LOW-LEVEL STEPPING (4 functions)                                       │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌──────────────────┐    ┌──────────────────┐                          │
  │  │ STEP_ALT_CW      │    │ STEP_ALT_CCW     │                          │
  │  └─────┬────────────┘    └─────┬────────────┘                          │
  │        ├─→ INC step idx         ├─→ DEC step idx                       │
  │        ├─→ AND 03h              ├─→ AND 03h                            │
  │        ├─→ Lookup pattern       ├─→ Lookup pattern                     │
  │        ├─→ OUT (ALT_PORT)       ├─→ OUT (ALT_PORT)                     │
  │        └─→ INC ALT_POS          └─→ DEC ALT_POS                        │
  │                                                                          │
  │  ┌──────────────────┐    ┌──────────────────┐                          │
  │  │ STEP_AZ_CW       │    │ STEP_AZ_CCW      │                          │
  │  └─────┬────────────┘    └─────┬────────────┘                          │
  │        ├─→ INC step idx         ├─→ DEC step idx                       │
  │        ├─→ AND 03h              ├─→ AND 03h                            │
  │        ├─→ Lookup pattern       ├─→ Lookup pattern                     │
  │        ├─→ OUT (AZ_PORT)        ├─→ OUT (AZ_PORT)                      │
  │        └─→ INC AZ_POS           └─→ DEC AZ_POS                         │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  MULTI-STEP MOVEMENT                                                     │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────┐            ┌─────────────────┐                    │
  │  │  MOVE_ALT       │            │  MOVE_AZ        │                    │
  │  │  Input: BC=steps│            │  Input: BC=steps│                    │
  │  └────┬────────────┘            └────┬────────────┘                    │
  │       │                              │                                  │
  │       ├─→ Check sign (bit 7 of B)    ├─→ Check sign                    │
  │       │   ┌─────────┬─────────┐      │   ┌─────────┬─────────┐        │
  │       │   │ Positive│Negative │      │   │ Positive│Negative │        │
  │       │   └────┬────┴────┬────┘      │   └────┬────┴────┬────┘        │
  │       │        │         │           │        │         │              │
  │       │   ┌────▼────┐ ┌──▼──────┐   │   ┌────▼────┐ ┌──▼──────┐      │
  │       │   │Loop:    │ │Two's    │   │   │Loop:    │ │Two's    │      │
  │       │   │STEP_CW  │ │comp BC  │   │   │STEP_CW  │ │comp BC  │      │
  │       │   │DELAY    │ │Loop:    │   │   │DELAY    │ │Loop:    │      │
  │       │   │DEC BC   │ │STEP_CCW │   │   │DEC BC   │ │STEP_CCW │      │
  │       │   │Until 0  │ │DELAY    │   │   │Until 0  │ │DELAY    │      │
  │       │   └─────────┘ │DEC BC   │   │   └─────────┘ │DEC BC   │      │
  │       │               └─────────┘   │               └─────────┘      │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  COORDINATED SLEW                                                        │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌───────────────────────┐                                              │
  │  │ SET_TARGET_POSITION   │                                              │
  │  │ Input: DE=alt, HL=az  │                                              │
  │  └──────┬────────────────┘                                              │
  │         ├─→ Store DE → ALT_TARGET_LO                                    │
  │         └─→ Store HL → AZ_TARGET_LO                                     │
  │                                                                          │
  │  ┌───────────────────────┐                                              │
  │  │ SLEW_TO_POSITION      │                                              │
  │  └──────┬────────────────┘                                              │
  │         │                                                                │
  │         ├─→ Set MOTOR_STATE bit 2 (slew active)                         │
  │         │                                                                │
  │         └─→ ┌─────────────────────────────────┐                         │
  │             │  SLEW_LOOP:                     │                         │
  │             ├─────────────────────────────────┤                         │
  │             │  1. Calc Alt error:             │                         │
  │             │     HL = TARGET - CURRENT       │                         │
  │             │     If HL ≠ 0:                  │                         │
  │             │       If HL > 0: STEP_ALT_CW    │                         │
  │             │       If HL < 0: STEP_ALT_CCW   │                         │
  │             │                                 │                         │
  │             │  2. Calc Az error:              │                         │
  │             │     HL = TARGET - CURRENT       │                         │
  │             │     If HL ≠ 0:                  │                         │
  │             │       If HL > 0: STEP_AZ_CW     │                         │
  │             │       If HL < 0: STEP_AZ_CCW    │                         │
  │             │                                 │                         │
  │             │  3. STEP_DELAY                  │                         │
  │             │                                 │                         │
  │             │  4. Check both at target?       │                         │
  │             │     NO  → loop back to 1        │                         │
  │             │     YES → continue              │                         │
  │             └─────────────────────────────────┘                         │
  │             │                                                            │
  │             └─→ Clear MOTOR_STATE bit 2                                 │
  │             └─→ Return                                                   │
  │                                                                          │
  │  ┌───────────────────────┐                                              │
  │  │ HOME_TELESCOPE        │                                              │
  │  └──────┬────────────────┘                                              │
  │         ├─→ SET_TARGET_POSITION(0, 0)                                   │
  │         └─→ SLEW_TO_POSITION                                            │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  POSITION QUERY                                                          │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────────────┐                                            │
  │  │ GET_CURRENT_POSITION    │                                            │
  │  │ Output: DE=alt, HL=az   │                                            │
  │  └─────┬───────────────────┘                                            │
  │        ├─→ Load DE from ALT_POS_LO                                      │
  │        └─→ Load HL from AZ_POS_LO                                       │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  DELAY FUNCTIONS                                                         │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────┐      ┌─────────────────┐                          │
  │  │  DELAY          │      │  STEP_DELAY     │                          │
  │  │  Input: BC      │      │  Input: A       │                          │
  │  └────┬────────────┘      └────┬────────────┘                          │
  │       │                        │                                        │
  │       └─→ Loop: DEC BC         └─→ BC=00+A                             │
  │           Until BC=0               Loop: DEC BC                         │
  │                                    Until BC=0                           │
  └──────────────────────────────────────────────────────────────────────────┘


  ┌─────────────────────────────────────────────────────────────────────────┐
  │                    4. ADDITIONAL ROUTINES IN MAIN.Z80                   │
  └─────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  DEGREES_TO_STEPS                                                        │
  │  Input: BC = degrees (0-360)                                            │
  │  Output: HL = steps                                                     │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────────────────────────────────┐                        │
  │  │  Formula: steps = (degrees × 56) / 10       │                        │
  │  └───────────────┬─────────────────────────────┘                        │
  │                  │                                                       │
  │                  ├─→ Multiply by 8 (shift left 3)                       │
  │                  ├─→ Multiply by 16 (shift left 4)                      │
  │                  ├─→ Multiply by 32 (shift left 5)                      │
  │                  ├─→ Add: 32 + 16 + 8 = 56                              │
  │                  ├─→ Call DIV16 (HL / 10)                               │
  │                  └─→ Return HL = result                                 │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  DIV16 - 16-bit Division                                                │
  │  Input: HL = dividend, BC = divisor                                     │
  │  Output: HL = quotient, DE = remainder                                  │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────────────────────────────────┐                        │
  │  │  Long division algorithm (16 iterations)     │                        │
  │  └───────────────┬─────────────────────────────┘                        │
  │                  │                                                       │
  │                  └─→ Loop 16 times:                                      │
  │                      ├─→ Shift dividend left                            │
  │                      ├─→ Shift into remainder                           │
  │                      ├─→ Try subtract divisor                           │
  │                      ├─→ If ≥ 0: keep subtraction, set quotient bit    │
  │                      └─→ If < 0: restore, clear quotient bit           │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  TEST ROUTINES                                                           │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌────────────────────┐    ┌────────────────────┐                      │
  │  │ TEST_ALT_MOTOR     │    │ TEST_AZ_MOTOR      │                      │
  │  └─────┬──────────────┘    └─────┬──────────────┘                      │
  │        │                          │                                     │
  │        ├─→ Move to ALT_MAX        ├─→ Rotate to AZ_MAX                 │
  │        │   (900 steps)            │   (2000 steps)                     │
  │        ├─→ Delay 10000            └─→ Return                           │
  │        ├─→ Move back to 0                                              │
  │        └─→ Return                                                       │
  │                                                                          │
  │  ┌─────────────────────┐                                                │
  │  │ TEST_BOTH_MOTORS    │                                                │
  │  └─────┬───────────────┘                                                │
  │        ├─→ Slew to (250, 500)                                           │
  │        ├─→ Delay 10000                                                  │
  │        ├─→ HOME_TELESCOPE                                               │
  │        └─→ Return                                                       │
  │                                                                          │
  │  ┌──────────────────────┐                                               │
  │  │ CALIBRATE_HOME       │                                               │
  │  │ (requires limit SW)  │                                               │
  │  └─────┬────────────────┘                                               │
  │        ├─→ Move Alt CCW until limit switch (STATUS_PORT bit 0)         │
  │        ├─→ Move Az CCW until limit switch (STATUS_PORT bit 1)          │
  │        ├─→ Zero both position counters                                  │
  │        └─→ Return                                                       │
  └──────────────────────────────────────────────────────────────────────────┘


  ┌─────────────────────────────────────────────────────────────────────────┐
  │                        SYSTEM OVERVIEW                                  │
  └─────────────────────────────────────────────────────────────────────────┘

    ┌──────────────────────────────────────────────────────────────────┐
    │                    Memory Map (8000h-800Ah)                      │
    ├──────────────────────────────────────────────────────────────────┤
    │  8000h-8001h │ ALT_POS      │ Current altitude position         │
    │  8002h-8003h │ AZ_POS       │ Current azimuth position          │
    │  8004h-8005h │ ALT_TARGET   │ Target altitude position          │
    │  8006h-8007h │ AZ_TARGET    │ Target azimuth position           │
    │  8008h       │ ALT_STEP_IDX │ Current step index (0-3) Alt     │
    │  8009h       │ AZ_STEP_IDX  │ Current step index (0-3) Az      │
    │  800Ah       │ MOTOR_STATE  │ Motor status flags                │
    └──────────────────────────────────────────────────────────────────┘

    ┌──────────────────────────────────────────────────────────────────┐
    │                    I/O Port Map (TEC-1)                          │
    ├──────────────────────────────────────────────────────────────────┤
    │  Port 00h    │ IN  │ Keyboard data (TEC-1 reserved)            │
    │  Port 01h    │ OUT │ Display digit select (TEC-1 reserved)     │
    │  Port 02h    │ OUT │ Display segment data (TEC-1 reserved)     │
    │  Port 03h    │ OUT │ Altitude motor (4-bit step pattern)       │
    │  Port 04h    │ OUT │ Azimuth motor (4-bit step pattern)        │
    │  Port 05h    │ IN  │ Status/control (optional - limit SW)      │
    └──────────────────────────────────────────────────────────────────┘

    ┌──────────────────────────────────────────────────────────────────┐
    │                    Step Pattern Table                            │
    ├──────────────────────────────────────────────────────────────────┤
    │  Step 0  │ 0001b  │ Coil A                                      │
    │  Step 1  │ 0010b  │ Coil B                                      │
    │  Step 2  │ 0100b  │ Coil A' (reversed)                         │
    │  Step 3  │ 1000b  │ Coil B' (reversed)                         │
    │                                                                  │
    │  Clockwise:     0 → 1 → 2 → 3 → 0  (increment, wrap at 4)      │
    │  Counterclockwise: 3 → 2 → 1 → 0 → 3  (decrement, wrap at 0)  │
    └──────────────────────────────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────────────────┐
  │                       4. KEYPAD-INPUT.Z80                               │
  │                 (TEC-1 Keypad Interface Module)                         │
  └─────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  NMI INTERRUPT HANDLER (at 0066H)                                        │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌─────────────────┐                                                    │
  │  │  NMI_HANDLER    │ ← 74C923 keypress triggers NMI                    │
  │  └────┬────────────┘                                                    │
  │       ├─→ Push AF, BC, DE, HL                                           │
  │       ├─→ IN A, (00H)  ; Read keypad                                    │
  │       ├─→ AND 1FH      ; Mask to 5 bits                                 │
  │       ├─→ Store in LAST_KEY                                             │
  │       ├─→ CALL PROCESS_KEY                                              │
  │       ├─→ Pop HL, DE, BC, AF                                            │
  │       └─→ RETN                                                           │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  KEY PROCESSING STATE MACHINE                                            │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌───────────────────────┐                                              │
  │  │  PROCESS_KEY          │                                              │
  │  └──────┬────────────────┘                                              │
  │         │                                                                │
  │         ├─→ Check for RS (E key)                                        │
  │         │   YES → HANDLE_RESET                                          │
  │         │         ├─→ Halt motors (output 0 to ports)                   │
  │         │         ├─→ Clear MOTOR_STATE                                 │
  │         │         ├─→ Preserve position counters                        │
  │         │         └─→ CALL INIT_INPUT_SYSTEM                            │
  │         │                                                                │
  │         ├─→ Get INPUT_MODE                                              │
  │         │   ┌──────┬──────┬──────┐                                      │
  │         │   │ 0    │ 1    │ 2    │                                      │
  │         │   │IDLE  │AD    │F     │                                      │
  │         │   └──┬───┴──┬───┴──┬───┘                                      │
  │         │      │      │      │                                          │
  │         ├──────▼      │      │                                          │
  │         │ HANDLE_IDLE_KEYS   │                                          │
  │         │  ├─→ AD key?       │                                          │
  │         │  │   Toggle ALT↔AZZ│                                          │
  │         │  │   Display selection                                        │
  │         │  │   Set MODE=1    │                                          │
  │         │  │                 │                                          │
  │         │  └─→ F key?        │                                          │
  │         │      Display "FAST"│                                          │
  │         │      Set MODE=2    │                                          │
  │         │                    │                                          │
  │         ├────────────────────▼                                          │
  │         │ HANDLE_AD_MODE_KEYS                                           │
  │         │  ┌─────────────────────────────┐                             │
  │         │  │ STATE_WAIT_SIGN             │                             │
  │         │  │  ├─→ + key: HANDLE_PLUS     │                             │
  │         │  │  │   Display "PLUS"         │                             │
  │         │  │  │   Set STATE=ENTER_DEG    │                             │
  │         │  │  │                           │                             │
  │         │  │  └─→ - key: HANDLE_MINUS    │                             │
  │         │  │      Display "NEG"          │                             │
  │         │  │      Set STATE=ENTER_DEG    │                             │
  │         │  │                              │                             │
  │         │  ├─────────────────────────────┤                             │
  │         │  │ STATE_ENTER_DEG             │                             │
  │         │  │  ├─→ Digit 0-9:             │                             │
  │         │  │  │   ADD_DIGIT_TO_BUFFER    │                             │
  │         │  │  │   DISPLAY_BUFFER         │                             │
  │         │  │  │                           │                             │
  │         │  │  ├─→ C key:                 │                             │
  │         │  │  │   Store 0 in DEG         │                             │
  │         │  │  │   Advance to ENTER_MIN   │                             │
  │         │  │  │                           │                             │
  │         │  │  └─→ GO key:                │                             │
  │         │  │      INC GO_COUNT           │                             │
  │         │  │      If GO_COUNT >= 2:      │                             │
  │         │  │        EXECUTE_MOVEMENT     │                             │
  │         │  │      Else:                  │                             │
  │         │  │        Store in DEG         │                             │
  │         │  │        Advance to ENTER_MIN │                             │
  │         │  │                              │                             │
  │         │  ├─────────────────────────────┤                             │
  │         │  │ STATE_ENTER_MIN             │                             │
  │         │  │  (Same as ENTER_DEG)        │                             │
  │         │  │  → Advances to ENTER_SEC    │                             │
  │         │  │                              │                             │
  │         │  ├─────────────────────────────┤                             │
  │         │  │ STATE_ENTER_SEC             │                             │
  │         │  │  (Same as ENTER_DEG)        │                             │
  │         │  │  → Goes to EXECUTE          │                             │
  │         │  └─────────────────────────────┘                             │
  │         │                                                               │
  │         └────────────────────▼                                          │
  │           HANDLE_F_MODE_KEYS                                            │
  │            ┌─────────────────────────────┐                             │
  │            │ STATE_IDLE (in F mode)      │                             │
  │            │  Wait for AD key            │                             │
  │            │  → Toggle ALT↔AZZ           │                             │
  │            │  → Set STATE=WAIT_SIGN      │                             │
  │            │                              │                             │
  │            ├─────────────────────────────┤                             │
  │            │ STATE_WAIT_SIGN             │                             │
  │            │  (Same +/- as AD mode)      │                             │
  │            │  → Set STATE=ENTER_DEG      │                             │
  │            │                              │                             │
  │            ├─────────────────────────────┤                             │
  │            │ STATE_ENTER_DEG (steps)     │                             │
  │            │  Enter step count 1-65535   │                             │
  │            │  → Advance to ENTER_SPEED   │                             │
  │            │                              │                             │
  │            ├─────────────────────────────┤                             │
  │            │ STATE_ENTER_SPEED           │                             │
  │            │  ├─→ Digit 1-9:             │                             │
  │            │  │   Store in INPUT_SPEED   │                             │
  │            │  │   DISPLAY_BUFFER         │                             │
  │            │  │                           │                             │
  │            │  ├─→ Digit 0:               │                             │
  │            │  │   Ignore (wait for 1-9)  │                             │
  │            │  │                           │                             │
  │            │  └─→ GO key:                │                             │
  │            │      EXECUTE_MOVEMENT       │                             │
  │            └─────────────────────────────┘                             │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  MOVEMENT EXECUTION                                                      │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌───────────────────────┐                                              │
  │  │ EXECUTE_MOVEMENT      │                                              │
  │  └──────┬────────────────┘                                              │
  │         │                                                                │
  │         ├─→ Check INPUT_MODE                                            │
  │         │   ┌─────────┬──────────┐                                      │
  │         │   │MODE 1   │MODE 2    │                                      │
  │         │   │(AD)     │(F)       │                                      │
  │         │   └────┬────┴────┬─────┘                                      │
  │         │        │         │                                            │
  │         │   ┌────▼────────────────────────┐                             │
  │         │   │ EXECUTE_AD_MODE             │                             │
  │         │   ├─────────────────────────────┤                             │
  │         │   │ 1. CONVERT_DMS_TO_STEPS     │                             │
  │         │   │    Formula:                 │                             │
  │         │   │    arc_sec = deg*3600 +     │                             │
  │         │   │              min*60 + sec   │                             │
  │         │   │    steps = arc_sec * 5 / 9  │                             │
  │         │   │                              │                             │
  │         │   │ 2. Apply sign (negate if -) │                             │
  │         │   │                              │                             │
  │         │   │ 3. Check axis:              │                             │
  │         │   │    ALT → CALL MOVE_ALT      │                             │
  │         │   │    AZZ → CALL MOVE_AZ       │                             │
  │         │   │                              │                             │
  │         │   │ 4. INIT_INPUT_SYSTEM        │                             │
  │         │   └─────────────────────────────┘                             │
  │         │                                                                │
  │         │   ┌────▼────────────────────────┐                             │
  │         │   │ EXECUTE_F_MODE              │                             │
  │         │   ├─────────────────────────────┤                             │
  │         │   │ 1. Get step count from      │                             │
  │         │   │    INPUT_DEG (reused)       │                             │
  │         │   │                              │                             │
  │         │   │ 2. Apply sign (negate if -) │                             │
  │         │   │                              │                             │
  │         │   │ 3. Get speed from           │                             │
  │         │   │    INPUT_SPEED → TEMP_SPEED │                             │
  │         │   │                              │                             │
  │         │   │ 4. Check axis:              │                             │
  │         │   │    ALT → MOVE_ALT_FAST      │                             │
  │         │   │    AZZ → MOVE_AZ_FAST       │                             │
  │         │   │                              │                             │
  │         │   │ 5. INIT_INPUT_SYSTEM        │                             │
  │         │   └─────────────────────────────┘                             │
  │         │                                                                │
  │         └─→ Return                                                       │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  FAST MOVEMENT ROUTINES                                                  │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  ┌────────────────────────┐      ┌────────────────────────┐            │
  │  │ MOVE_ALT_FAST          │      │ MOVE_AZ_FAST           │            │
  │  │ Input: BC = steps      │      │ Input: BC = steps      │            │
  │  └───────┬────────────────┘      └───────┬────────────────┘            │
  │          │                               │                              │
  │          ├─→ Check sign (bit 7)          ├─→ Check sign               │
  │          │   ┌─────────┬─────────┐       │   ┌─────────┬─────────┐    │
  │          │   │Positive │Negative │       │   │Positive │Negative │    │
  │          │   └────┬────┴────┬────┘       │   └────┬────┴────┬────┘    │
  │          │        │         │            │        │         │          │
  │          │   ┌────▼─────────▼────────┐   │   ┌────▼─────────▼────┐    │
  │          │   │ Loop until BC = 0:    │   │   │ Loop until BC=0:  │    │
  │          │   │  ├─→ STEP_ALT_CW/CCW  │   │   │  ├─→ STEP_AZ_CW/CCW│   │
  │          │   │  ├─→ CALL FAST_DELAY  │   │   │  ├─→ FAST_DELAY   │    │
  │          │   │  ├─→ DEC BC           │   │   │  ├─→ DEC BC       │    │
  │          │   │  └─→ Loop             │   │   │  └─→ Loop         │    │
  │          │   └───────────────────────┘   │   └───────────────────┘    │
  │          │                                │                             │
  │          └────────────────────────────────┘                             │
  │                                                                          │
  │  ┌────────────────────────┐                                             │
  │  │ FAST_DELAY             │                                             │
  │  └───────┬────────────────┘                                             │
  │          │                                                               │
  │          ├─→ Get SLEW_DELAY (base)                                      │
  │          ├─→ Get TEMP_SPEED (multiplier)                                │
  │          ├─→ If TEMP_SPEED = 1:                                         │
  │          │     Use SLEW_DELAY                                           │
  │          │   Else:                                                      │
  │          │     Divide: delay = SLEW_DELAY / TEMP_SPEED                  │
  │          └─→ CALL STEP_DELAY with result                                │
  └──────────────────────────────────────────────────────────────────────────┘

  ┌──────────────────────────────────────────────────────────────────────────┐
  │  DISPLAY ROUTINES                                                        │
  ├──────────────────────────────────────────────────────────────────────────┤
  │                                                                          │
  │  Text Displays:                                                          │
  │   ┌──────────────────┬──────────────────┬──────────────────┐            │
  │   │ DISPLAY_ALT      │ DISPLAY_AZZ      │ DISPLAY_PLUS     │            │
  │   │ Shows: "ALT   "  │ Shows: "AZZ   "  │ Shows: "PLUS  "  │            │
  │   └──────────────────┴──────────────────┴──────────────────┘            │
  │                                                                          │
  │   ┌──────────────────┬──────────────────┬──────────────────┐            │
  │   │ DISPLAY_NEG      │ DISPLAY_FAST     │ CLEAR_DISPLAY    │            │
  │   │ Shows: "NEG   "  │ Shows: "FAST  "  │ Blanks all       │            │
  │   └──────────────────┴──────────────────┴──────────────────┘            │
  │                                                                          │
  │  Number Display:                                                         │
  │   ┌──────────────────────────────────────────┐                          │
  │   │ DISPLAY_NUMBER                           │                          │
  │   │ Input: HL = 16-bit number (0-65535)      │                          │
  │   ├──────────────────────────────────────────┤                          │
  │   │ 1. Extract digits via DIV16 by 10        │                          │
  │   │    Loop 6 times:                         │                          │
  │   │      HL = HL / 10, remainder = digit     │                          │
  │   │      Store digit in DIGIT_BUFFER         │                          │
  │   │                                           │                          │
  │   │ 2. Display digits right-justified:       │                          │
  │   │    For each digit position (0-5):        │                          │
  │   │      SELECT_DIGIT (via port 01h)         │                          │
  │   │      Lookup segment pattern              │                          │
  │   │      OUT to port 02h                     │                          │
  │   └──────────────────────────────────────────┘                          │
  │                                                                          │
  │  7-Segment Encoding (from tec1-sbc-spec.md):                            │
  │   Non-sequential: a=D0, f=D1, g=D2, b=D3, dp=D4, c=D5, e=D6, d=D7      │
  │                                                                          │
  │   Example patterns:                                                      │
  │     '0' = 0xEB    'A' = 0x6F    'L' = 0xC2                              │
  │     '5' = 0xA7    'P' = 0x4F    'U' = 0xE2                              │
  └──────────────────────────────────────────────────────────────────────────┘

  Summary of Programs:

  1. simple-test.z80 - Standalone test program, runs 4 sequential tests
  2. main.z80 - Full featured program with keypad interface and examples
  3. stepper-lib.z80 - Reusable library with all motor control functions
  4. config.z80 - Configuration constants (not executable)
  5. keypad-input.z80 - TEC-1 keypad interface with dual input modes


