// Initialize variables
1 p !                            // p = Port number for encoder signal A
2 q !                            // q = Port number for encoder signal B
[0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0 ] o !  // o = Direction array for quadrature decoding
1500 m !                         // m = Maximum pulse count for 90 degrees (6000 pulses per 360 degrees)
0 c !                            // c = Pulse count initialized to 0

3 f_d !                          // f_d = Degrees factor numerator
50 f_div !                       // f_div = Degrees divisor denominator

// Read initial encoder state
p /I s !                         // Read signal A into s
q /I t !                         // Read signal B into t
s t { | a !                      // Combine s and t into previous state 'a'

// Define interrupt handler for encoder pulses
:Z
  // Read current encoder state
  p /I s !                       // Read signal A into s
  q /I t !                       // Read signal B into t
  s t { | b !                    // Combine s and t into current state 'b'

  // Decode quadrature signals
  a 4 * b + n !                  // n = (a << 2) + b, transition code
  o n ? r !                      // r = Direction from direction array o[n]

  // Update pulse count based on direction
  r 0 > (
    c 1 + c !                    // If r > 0, increment pulse count 'c'
  ) /E (
    r 0 < (
      c 1 - c !                  // If r < 0, decrement pulse count 'c'
    )
  )

  // Ensure pulse count is within 0 and maximum count
  c 0 < (
    0 c !                        // If c < 0, set c to 0
  )
  c m > (
    m c !                        // If c > m, set c to maximum count 'm'
  )

  // Update previous state
  a b !                          // Set previous state 'a' to current state 'b'

  // Convert pulse count to degrees, minutes, and seconds
  c f_d * d !                    // d = c * f_d
  d f_div / deg !                // deg = (c * f_d) / f_div

  // Calculate remaining pulses after degrees
  d f_div % rem_p !              // rem_p = (c * f_d) % f_div

  // Calculate minutes
  rem_p 60 * m1 !                // m1 = rem_p * 60
  m1 f_div / min !               // min = (rem_p * 60) / f_div

  // Calculate remaining pulses after minutes
  m1 f_div % rem_s !             // rem_s = (rem_p * 60) % f_div

  // Calculate seconds
  rem_s 60 * s1 !                // s1 = rem_s * 60
  s1 f_div / sec !               // sec = (rem_s * 60) / f_div

  // Output the angle
  `Degrees: ` deg . /N           // Print degrees
  `Minutes: ` min . /N           // Print minutes
  `Seconds: ` sec . /N           // Print seconds
;

// Main program can perform other tasks here
// For demonstration, we'll loop and print a message
/U (
  `Main program is running other tasks...` /N
  // Simulate other processing
  1000 ( )                       // Wait loop (adjust as needed)
)
