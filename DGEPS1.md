```
// TEC-1 Dual Geared Encoder Position System
// For 600ppr encoders with 10:1 reduction gearboxes
// Resolution: 0.015° per count (24000 counts/rev)
// Inputs: Ports 2,3 (Encoder1 A,B), Ports 4,5 (Encoder2 A,B)
// Controls: ESC to exit, F to reset counters
//  
// Date: Jan 2025

// Port assignments for quadrature inputs
2 a! // Encoder 1 - A pulse input
3 b! // Encoder 1 - B pulse input
4 c! // Encoder 2 - A pulse input
5 d! // Encoder 2 - B pulse input

// Initialize counting variables
0 e! // Encoder 1 position counter
0 f! // Encoder 2 position counter
0 g! // Encoder 1 previous state
0 h! // Encoder 2 previous state

// Angle calculation constant
// 600 pulses × 4 (quadrature) × 10 (gear) = 24000 pulses/rev
// 360° / 24000 = 0.015° per pulse
// Scale by 1000 for 3 decimal places: 0.015 × 1000 = 15
15 j! // Degrees per 1000 pulses

// Quadrature state decoder function
// Takes combined state (old state in high nibble, new in low)
// Returns: 1 for CW, -1 for CCW, 0 for invalid
:Q k!
  k #0F &          // Mask to get new state
  k 4 } #0F &      // Shift and mask to get old state
  2 * +            // Combine states for table lookup
  // Lookup table for all possible state transitions
  [0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0] $! 
  $ k ? .          // Get direction from table
;

// Function to read and process encoder 1
:A 
  a /I 2 * b /I +  // Combine current A,B states
  g 4 * +          // Combine with previous state
  " Q              // Get direction from decoder
  " 0 = /F (       // If valid transition
    e $ + e!       // Update position counter
  )
  g!               // Save current state
;

// Function to read and process encoder 2
:B
  c /I 2 * d /I +  // Combine current A,B states
  h 4 * +          // Combine with previous state
  " Q              // Get direction from decoder
  " 0 = /F (       // If valid transition
    f $ + f!       // Update position counter
  )
  h!               // Save current state
;

// Convert count to degrees with 3 decimal places
:D k!              // Input: count in k
  k j *            // Scale by degrees/pulse
  1000 /           // Convert to actual degrees
;

// Format and display angle
:E k!              // Input: count in k
  k D " 1000 /     // Get whole degrees
  `.`              // Decimal point
  k D 1000 % " 100 / . // Show 3 decimal places
  ` degrees`       // Add units
;

// Main monitoring loop
:C
  /T i!            // Initialize loop control
  /U (             // Start unlimited loop
    i /W           // While control is true
    A B            // Read both encoders
    `Encoder 1: Count:` e . 
    ` Angle:` e E /N
    `Encoder 2: Count:` f . 
    ` Angle:` f E /N
    50()           // Delay for readability
    /K 27 = (      // Check for ESC key
      /F i!        // Exit if pressed
    )
  )
;

// Reset function
:F 
  0 e! 0 f!       // Clear both counters
  `Counters reset to zero` /N
;

// Type C to start monitoring, F to reset, ESC to exit



```

 
# pseudocode conversion ( of above) 
- with comments noting potential issues or considerations:

```
// TEC-1 Dual Geared Encoder Position System
// For 600ppr encoders with 10:1 reduction gearboxes
// Resolution: 0.015° per count (24000 counts/rev)
// Inputs: Ports 2,3 (Encoder1 A,B), Ports 4,5 (Encoder2 A,B)
// Controls: ESC to exit, F to reset counters
// Date: Jan 2025

// Define constants
CONST ENCODER1_A = 2    // Port 2: Encoder 1 A pulse input
CONST ENCODER1_B = 3    // Port 3: Encoder 1 B pulse input
CONST ENCODER2_A = 4    // Port 4: Encoder 2 A pulse input
CONST ENCODER2_B = 5    // Port 5: Encoder 2 B pulse input
CONST DEG_PER_PULSE = 15  // 0.015° * 1000 for 3 decimal places

// Global variables
INTEGER enc1_count = 0    // Encoder 1 position counter
INTEGER enc2_count = 0    // Encoder 2 position counter
INTEGER enc1_prev = 0     // Encoder 1 previous state
INTEGER enc2_prev = 0     // Encoder 2 previous state

// Function: Quadrature Decoder
// Input: 8-bit value with old state (high nibble) and new state (low nibble)
// Returns: 1 (CW), -1 (CCW), or 0 (invalid transition)
FUNCTION quad_decode(state):
    new_state = state AND 0x0F
    old_state = (state >> 4) AND 0x0F
    index = old_state * 2 + new_state
    lookup = [0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0]
    RETURN lookup[index]
END FUNCTION

// Function: Process Encoder 1
FUNCTION process_enc1():
    current = (read_port(ENCODER1_A) * 2) + read_port(ENCODER1_B)
    combined = (enc1_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        enc1_count = enc1_count + direction
    END IF
    enc1_prev = current
END FUNCTION

// Function: Process Encoder 2
FUNCTION process_enc2():
    current = (read_port(ENCODER2_A) * 2) + read_port(ENCODER2_B)
    combined = (enc2_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        enc2_count = enc2_count + direction
    END IF
    enc2_prev = current
END FUNCTION

// Function: Convert counts to degrees
FUNCTION counts_to_degrees(count):
    RETURN (count * DEG_PER_PULSE) / 1000
END FUNCTION

// Function: Display formatted angle
FUNCTION display_angle(count):
    degrees = counts_to_degrees(count)
    whole_deg = degrees / 1000
    decimals = (degrees MOD 1000) / 100  // First decimal place only?
    PRINT whole_deg + "." + decimals + " degrees"
END FUNCTION

// Function: Main monitoring loop
FUNCTION monitor():
    running = TRUE
    WHILE running:
        process_enc1()
        process_enc2()
        PRINT "Encoder 1: Count: " + enc1_count
        PRINT " Angle: "
        display_angle(enc1_count)
        PRINT "Encoder 2: Count: " + enc2_count
        PRINT " Angle: "
        display_angle(enc2_count)
        delay(50)    // 50ms delay for display readability
        IF key_pressed() == 27 THEN    // ESC key
            running = FALSE
        END IF
        IF key_pressed() == 'F' THEN    // F key
            reset_counters()
        END IF
    END WHILE
END FUNCTION

// Function: Reset counters
FUNCTION reset_counters():
    enc1_count = 0
    enc2_count = 0
    PRINT "Counters reset to zero"
END FUNCTION

// Main program
monitor()    // Start monitoring when program runs
```

### Comments on Issues/Problems:

1. **Key Handling**: 
   - The original code only checks for ESC in the main loop, but F reset is mentioned in comments. Added F key check, but simultaneous key detection might need clarification.
   - Potential issue: No debouncing or key repeat handling specified.

2. **Precision**:
   - The display_angle function only shows one decimal place in the pseudocode (due to /100), while the original implies three (/1000 % formatting). This might be a display limitation or error.
   - Integer arithmetic might lead to rounding errors when converting to degrees.

3. **Interrupt Handling**:
   - The original code doesn’t specify if encoder reading is interrupt-driven or polled. Pseudocode assumes polling, which might miss counts at high speeds.

4. **Variable Scope**:
   - Global variables are used throughout. In a real implementation, consider scope and thread safety if interrupts are added.

5. **Port Reading**:
   - `read_port()` is assumed but not defined. Actual hardware implementation would need specific I/O functions.

6. **Overflow**:
   - No bounds checking on `enc1_count` and `enc2_count`. Large rotations could cause integer overflow depending on variable size.

7. **Timing**:
   - 50ms delay might be too long for fast encoder changes or too short for display readability, depending on hardware.

8. **Initialization**:
   - No explicit initialization of port directions (input/output) shown, assumed handled by hardware.

This pseudocode provides a structured interpretation while maintaining the original functionality, with notes on potential improvements or clarifications needed for a real implementation.


# updated pseudocode
- incorporating signed 16-bit integer logic,
- with explicit handling for carry, remainders, and bit rollovers.

```
// TEC-1 Dual Geared Encoder Position System
// For 600ppr encoders with 10:1 reduction gearboxes
// Resolution: 0.015° per count (24000 counts/rev)
// Inputs: Ports 2,3 (Encoder1 A,B), Ports 4,5 (Encoder2 A,B)
// Controls: ESC to exit, F to reset counters
// Date: Jan 2025
// Note: Using signed 16-bit integers (-32768 to 32767)

// Define constants
CONST ENCODER1_A = 2    // Port 2: Encoder 1 A pulse input
CONST ENCODER1_B = 3    // Port 3: Encoder 1 B pulse input
CONST ENCODER2_A = 4    // Port 4: Encoder 2 A pulse input
CONST ENCODER2_B = 5    // Port 5: Encoder 2 B pulse input
CONST DEG_PER_PULSE = 15  // 0.015° * 1000 for 3 decimal places
CONST MAX_INT16 = 32767   // Maximum positive value for 16-bit signed
CONST MIN_INT16 = -32768  // Minimum negative value for 16-bit signed

// Global variables (signed 16-bit)
INT16 enc1_count = 0    // Encoder 1 position counter
INT16 enc2_count = 0    // Encoder 2 position counter
INT16 enc1_prev = 0     // Encoder 1 previous state
INT16 enc2_prev = 0     // Encoder 2 previous state

// Function: Quadrature Decoder
// Input: 8-bit value with old state (high nibble) and new state (low nibble)
// Returns: 1 (CW), -1 (CCW), or 0 (invalid transition)
FUNCTION quad_decode(state):
    new_state = state AND 0x0F
    old_state = (state >> 4) AND 0x0F
    index = old_state * 2 + new_state
    lookup = [0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0]
    RETURN lookup[index]
END FUNCTION

// Function: Process Encoder 1 with overflow handling
FUNCTION process_enc1():
    current = (read_port(ENCODER1_A) * 2) + read_port(ENCODER1_B)
    combined = (enc1_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        // Handle 16-bit signed overflow
        IF direction == 1 AND enc1_count == MAX_INT16 THEN
            enc1_count = MIN_INT16    // Rollover from 32767 to -32768
        ELSE IF direction == -1 AND enc1_count == MIN_INT16 THEN
            enc1_count = MAX_INT16    // Rollover from -32768 to 32767
        ELSE
            enc1_count = enc1_count + direction    // Normal increment/decrement
        END IF
    END IF
    enc1_prev = current
END FUNCTION

// Function: Process Encoder 2 with overflow handling
FUNCTION process_enc2():
    current = (read_port(ENCODER2_A) * 2) + read_port(ENCODER2_B)
    combined = (enc2_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        // Handle 16-bit signed overflow
        IF direction == 1 AND enc2_count == MAX_INT16 THEN
            enc2_count = MIN_INT16    // Rollover from 32767 to -32768
        ELSE IF direction == -1 AND enc2_count == MIN_INT16 THEN
            enc2_count = MAX_INT16    // Rollover from -32768 to 32767
        ELSE
            enc2_count = enc2_count + direction    // Normal increment/decrement
        END IF
    END IF
    enc2_prev = current
END FUNCTION

// Function: Convert counts to degrees (handling 16-bit range)
// Returns degrees * 1000 to maintain precision
FUNCTION counts_to_degrees(count):
    // 24000 counts = 360°, so full range (-32768 to 32767) maps to -491.52° to 491.505°
    RETURN (count * DEG_PER_PULSE)    // Keep as int32 to avoid overflow
END FUNCTION

// Function: Display formatted angle
FUNCTION display_angle(count):
    scaled_deg = counts_to_degrees(count)    // INT32 to hold full range
    whole_deg = scaled_deg / 1000
    decimals = ABS(scaled_deg MOD 1000)      // Handle negative values
    PRINT whole_deg + "." + decimals + " degrees"
END FUNCTION

// Function: Main monitoring loop
FUNCTION monitor():
    running = TRUE
    WHILE running:
        process_enc1()
        process_enc2()
        PRINT "Encoder 1: Count: " + enc1_count
        PRINT " Angle: "
        display_angle(enc1_count)
        PRINT "Encoder 2: Count: " + enc2_count
        PRINT " Angle: "
        display_angle(enc2_count)
        delay(50)    // 50ms delay for display readability
        IF key_pressed() == 27 THEN    // ESC key
            running = FALSE
        END IF
        IF key_pressed() == 'F' THEN    // F key
            reset_counters()
        END IF
    END WHILE
END FUNCTION

// Function: Reset counters
FUNCTION reset_counters():
    enc1_count = 0
    enc2_count = 0
    PRINT "Counters reset to zero"
END FUNCTION

// Main program
monitor()    // Start monitoring when program runs
```
# comments on above
Here's the updated pseudocode incorporating signed 16-bit integer logic, with explicit handling for carry, remainders, and bit rollovers. I'll add comments highlighting changes and potential issues:

```
// TEC-1 Dual Geared Encoder Position System
// For 600ppr encoders with 10:1 reduction gearboxes
// Resolution: 0.015° per count (24000 counts/rev)
// Inputs: Ports 2,3 (Encoder1 A,B), Ports 4,5 (Encoder2 A,B)
// Controls: ESC to exit, F to reset counters
// Date: Jan 2025
// Note: Using signed 16-bit integers (-32768 to 32767)

// Define constants
CONST ENCODER1_A = 2    // Port 2: Encoder 1 A pulse input
CONST ENCODER1_B = 3    // Port 3: Encoder 1 B pulse input
CONST ENCODER2_A = 4    // Port 4: Encoder 2 A pulse input
CONST ENCODER2_B = 5    // Port 5: Encoder 2 B pulse input
CONST DEG_PER_PULSE = 15  // 0.015° * 1000 for 3 decimal places
CONST MAX_INT16 = 32767   // Maximum positive value for 16-bit signed
CONST MIN_INT16 = -32768  // Minimum negative value for 16-bit signed

// Global variables (signed 16-bit)
INT16 enc1_count = 0    // Encoder 1 position counter
INT16 enc2_count = 0    // Encoder 2 position counter
INT16 enc1_prev = 0     // Encoder 1 previous state
INT16 enc2_prev = 0     // Encoder 2 previous state

// Function: Quadrature Decoder
// Input: 8-bit value with old state (high nibble) and new state (low nibble)
// Returns: 1 (CW), -1 (CCW), or 0 (invalid transition)
FUNCTION quad_decode(state):
    new_state = state AND 0x0F
    old_state = (state >> 4) AND 0x0F
    index = old_state * 2 + new_state
    lookup = [0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0]
    RETURN lookup[index]
END FUNCTION

// Function: Process Encoder 1 with overflow handling
FUNCTION process_enc1():
    current = (read_port(ENCODER1_A) * 2) + read_port(ENCODER1_B)
    combined = (enc1_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        // Handle 16-bit signed overflow
        IF direction == 1 AND enc1_count == MAX_INT16 THEN
            enc1_count = MIN_INT16    // Rollover from 32767 to -32768
        ELSE IF direction == -1 AND enc1_count == MIN_INT16 THEN
            enc1_count = MAX_INT16    // Rollover from -32768 to 32767
        ELSE
            enc1_count = enc1_count + direction    // Normal increment/decrement
        END IF
    END IF
    enc1_prev = current
END FUNCTION

// Function: Process Encoder 2 with overflow handling
FUNCTION process_enc2():
    current = (read_port(ENCODER2_A) * 2) + read_port(ENCODER2_B)
    combined = (enc2_prev * 4) + current
    direction = quad_decode(combined)
    IF direction != 0 THEN
        // Handle 16-bit signed overflow
        IF direction == 1 AND enc2_count == MAX_INT16 THEN
            enc2_count = MIN_INT16    // Rollover from 32767 to -32768
        ELSE IF direction == -1 AND enc2_count == MIN_INT16 THEN
            enc2_count = MAX_INT16    // Rollover from -32768 to 32767
        ELSE
            enc2_count = enc2_count + direction    // Normal increment/decrement
        END IF
    END IF
    enc2_prev = current
END FUNCTION

// Function: Convert counts to degrees (handling 16-bit range)
// Returns degrees * 1000 to maintain precision
FUNCTION counts_to_degrees(count):
    // 24000 counts = 360°, so full range (-32768 to 32767) maps to -491.52° to 491.505°
    RETURN (count * DEG_PER_PULSE)    // Keep as int32 to avoid overflow
END FUNCTION

// Function: Display formatted angle
FUNCTION display_angle(count):
    scaled_deg = counts_to_degrees(count)    // INT32 to hold full range
    whole_deg = scaled_deg / 1000
    decimals = ABS(scaled_deg MOD 1000)      // Handle negative values
    PRINT whole_deg + "." + decimals + " degrees"
END FUNCTION

// Function: Main monitoring loop
FUNCTION monitor():
    running = TRUE
    WHILE running:
        process_enc1()
        process_enc2()
        PRINT "Encoder 1: Count: " + enc1_count
        PRINT " Angle: "
        display_angle(enc1_count)
        PRINT "Encoder 2: Count: " + enc2_count
        PRINT " Angle: "
        display_angle(enc2_count)
        delay(50)    // 50ms delay for display readability
        IF key_pressed() == 27 THEN    // ESC key
            running = FALSE
        END IF
        IF key_pressed() == 'F' THEN    // F key
            reset_counters()
        END IF
    END WHILE
END FUNCTION

// Function: Reset counters
FUNCTION reset_counters():
    enc1_count = 0
    enc2_count = 0
    PRINT "Counters reset to zero"
END FUNCTION

// Main program
monitor()    // Start monitoring when program runs
```

### Comments on Signed 16-bit Integer Logic:

1. **Range Limitation**:
   - A 16-bit signed integer (-32768 to 32767) covers ±32767 counts. With 24000 counts per revolution (600 × 4 × 10), this is only about ±1.36 revolutions before rollover. This might be insufficient for some applications.

2. **Overflow Handling**:
   - Explicitly handled rollovers in `process_enc1` and `process_enc2`. When incrementing at 32767, it rolls to -32768, and decrementing at -32768 rolls to 32767, mimicking 16-bit hardware behavior.

3. **Carry and Arithmetic**:
   - In `counts_to_degrees`, the multiplication `count * DEG_PER_PULSE` could exceed 16-bit range (-491520 to 491505), so it’s implicitly treated as a 32-bit intermediate result to preserve accuracy before division.

4. **Remainders**:
   - The `display_angle` function uses `MOD 1000` to extract decimals. With negative numbers, `ABS` ensures proper display formatting.

5. **Bit Rollovers**:
   - The rollover logic preserves continuity, but the jump from 32767 to -32768 (or vice versa) represents a sudden ±491.52° shift in angle, which might confuse users unless tracked externally.

6. **Resolution vs. Range Trade-off**:
   - Sticking to 16-bit limits resolution to ±491.52°, far less than a typical encoder’s capability with gearboxes. Consider if 32-bit counters would be more practical for real-world use.

7. **Potential Issues**:
   - **Speed**: Polling might miss counts near rollover points if the encoder moves quickly during the 50ms delay.
   - **Display**: The angle jumps at rollover might need additional logic (e.g., tracking total revolutions) for a smoother user experience.
   - **Precision**: Integer division in `counts_to_degrees` discards remainders unless scaled properly; current scaling avoids this but assumes 32-bit intermediates.

This version ensures 16-bit signed integer compliance with proper overflow handling, but the limited range might necessitate adjustments depending on the application’s requirements.
