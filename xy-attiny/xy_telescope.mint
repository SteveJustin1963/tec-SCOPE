// xy_telescope.mint
// MINT 2 telescope controller — motor control + position goto/tracking
// TEC-1 console TX is T-junctioned to ATtiny84A RX
// ATtiny84A sniffs motor commands and sends back position as: A<alt> <az>\n
//
// ── Variable map ──────────────────────────────────────────────────────────
//   d  delay constant: loop iterations per ~1ms (tune for your board)
//   t  scratch: remainder after /100 in D
//   u  scratch: units digit in D
//   f  fast slew speed (default 80) — used when error > b
//   s  slow approach speed (default 20) — used when error in (e, b]
//   b  coarse speed boundary in tenths of deg (default 100 = 10.0°)
//   e  arrival threshold in tenths of deg (default 20 = 2.0°)
//   a  current altitude in tenths of deg (e.g. 325 = 32.5°)
//   z  current azimuth  in tenths of deg (e.g. 1573 = 157.3°)
//   p  target altitude in tenths of deg
//   q  target azimuth  in tenths of deg
//   o  star table array pointer
//
// ── Function map ──────────────────────────────────────────────────────────
//  HELPERS
//   D  emit 3-digit zero-padded decimal       ( n -- )
//   M  emit full motor command                ( speed axis dir -- )
//  MOTOR CONTROL
//   F  X forward                              ( speed -- )
//   R  X reverse                              ( speed -- )
//   H  X stop                                 ( -- )
//   G  Y forward                              ( speed -- )
//   V  Y reverse                              ( speed -- )
//   J  Y stop                                 ( -- )
//   S  stop all axes                          ( -- )
//   X  slew X forward N steps                 ( speed steps -- )
//   Y  slew Y forward N steps                 ( speed steps -- )
//   A  nudge X forward                        ( -- )
//   B  nudge X reverse                        ( -- )
//   C  nudge Y forward                        ( -- )
//   E  nudge Y reverse                        ( -- )
//  POSITION CONTROL
//   N  read 3-digit decimal from serial       ( -- n )
//   I  read position update from ATtiny84A    ( -- )  stores in a, z
//   P  print current position to console      ( -- )
//   K  set target from stack                  ( alt az -- )
//   L  look up star n in table, set target    ( n -- )
//   O  single altitude correction step        ( -- )
//   Q  single azimuth correction step         ( -- )
//   W  goto: drive to target p,q              ( -- )
//   T  track: continuous correction           ( -- )  runs until RESET
//
// ── Star table ────────────────────────────────────────────────────────────
//   [az alt  az alt  az alt  az alt  az alt]  — all values in tenths of degrees
//    0:Sirius 1:Canopus 2:Achernar 3:AlphaCen 4:Rigel
//   e.g. az=1060 means 106.0°,  alt=350 means 35.0°
//   Approx. Melbourne, Australia (~37S), January 9pm AEST
//   UPDATE these values for your location and observation time
//
// ── Usage examples ────────────────────────────────────────────────────────
//   I P              read and display current position (values in tenths of deg)
//   325 1573 K W     goto altitude 32.5, azimuth 157.3
//   320 1570 K W     goto altitude 32.0, azimuth 157.0
//   0 L W            point at Sirius (uses tenths from star table)
//   T                track current target (runs until RESET)
//
// Delay tuning: adjust d so that  1 d * ()  takes approx 1ms
// Strip all comment lines before uploading to TEC-1

// ── Initialise variables ──────────────────────────────────────────────────
100 d!
80 f!
20 s!
100 b!
20 e!
[1060 350 1730 520 2140 500 2250 350 2800 350] o!

// ── Helpers ───────────────────────────────────────────────────────────────

// D: emit 3-digit zero-padded ASCII decimal  ( n -- )
// Saves /r to t before any arithmetic that would clobber it
:D 100 / /r t! 48 + /C t 10 / /r u! 48 + /C u 48 + /C ;

// M: emit motor command bytes  ( speed axis dir -- )
:M /C /C D ;

// ── Motor control ─────────────────────────────────────────────────────────

:F 88 70 M ;          // X forward
:R 88 82 M ;          // X reverse
:H 0 F ;              // X stop

:G 89 70 M ;          // Y forward
:V 89 82 M ;          // Y reverse
:J 0 G ;              // Y stop

:S H J ;              // emergency stop both axes

// Slew: run motor for steps*d loop counts then stop
:X $ F d * () H ;
:Y $ G d * () J ;

// Nudges: speed 30, ~50ms duration
:A 30 50 X ;
:B 30 R 50 d * () H ;
:C 30 50 Y ;
:E 30 V 50 d * () J ;

// ── Position control ──────────────────────────────────────────────────────

// N: read 4-digit zero-padded decimal from serial  ( -- n )
// Reads tenths-of-degree values: alt 0000-0900, az 0000-3599
:N 0 4 ( 10 * /K 48 - + ) ;

// I: block until ATtiny84A sends 'A', then read alt->a, az->z  ( -- )
:I /U ( /K 65 - /W ) N a! /K ' N z! ;

// P: print current position  ( -- )
// Values shown in tenths of degrees: 325 = 32.5 degrees
:P `ALT=` a . `AZ=` z . ;

// K: set target position  ( alt az -- )
:K q! p! ;

// L: load star n as target  ( n -- )
// Star table is [az alt az alt...]; index n*2=az, n*2+1=alt
:L " 2 * o $ ? q! 2 * 1 + o $ ? p! ;

// O: single altitude correction step  ( -- )
// Two-speed: fast (f) when error > b tenths (100=10°), slow (s) in (e,b], stop when <= e tenths (20=2°)
:O p a - " 0 < ( -1 * ) " b > ( ' p a > ( f G ) /E ( f V ) ) /E ( e > ( p a > ( s G ) /E ( s V ) ) /E ( J ) ) ;

// Q: single azimuth correction step  ( -- )
// Skips azimuth near zenith (a>85); two-speed same as O
:Q a 85 > ( H ) /E ( q z - " 0 < ( -1 * ) " b > ( ' q z > ( f F ) /E ( f R ) ) /E ( e > ( q z > ( s F ) /E ( s R ) ) /E ( H ) ) ) ;

// W: goto — drive both axes until within e degrees of target  ( -- )
:W /U ( I p a - " 0 < ( -1 * ) e > q z - " 0 < ( -1 * ) e > | /W O Q ) S ;

// T: track — continuous slow correction loop  ( -- )  runs until RESET
// Sets both speeds to 20 so tracking never triggers the fast-slew zone
:T 20 f! 20 s! /U ( I P O Q 500() ) ;
