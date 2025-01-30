// TEC-1 Dual Geared Encoder Position System
// For 600ppr encoders with 10:1 reduction gearboxes
// Resolution: 0.015° per count (24000 counts/rev)
// Inputs: Ports 2,3 (Encoder1 A,B), Ports 4,5 (Encoder2 A,B)
// Controls: ESC to exit, F to reset counters
 
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
