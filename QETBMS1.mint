// new version 21 july 2025

// 10:1 Geared Quadrature Encoder Simulator
// Tests 600 pulse/rev encoder with CW/CCW motion
// First 11 steps clockwise, next 9 counter-clockwise
// Resolution: 0.015 degrees per count
// (360° / (600 pulses × 4 quad × 10 gear ratio))
// Test pattern array:
// CW sequence:  00->01->11->10 (0,1,3,2)
// CCW sequence: 11->01->00->10 (3,1,0,2)

[0 1 3 2 0 1 3 2 0 1 3 1 0 2 3 1 0 2 3 1] p!

// Initialize variables
0 r!  // Pattern array index
0 e!  // Encoder count value  
0 g!  // Previous state storage

// Function T: Get current state and extract A,B bits
:T 
r p ? d!       // Get state from pattern array into d
d 1 & b!       // Extract B bit (LSB)
d 2 / 1 & a!   // Extract A bit (MSB)
;

// Function U: Update counter based on direction
:U 
r 11 < (       // First 11 steps = Clockwise
    e 1 + e!   // Increment counter
) /E (         // Last 9 steps = Counter-clockwise
    e 1 - e!   // Decrement counter
)
d g!           // Store current state for next comparison
;

// Function V: Display formatted output
:V
`Step ` r 1 + .           // Show step number (1-based)
`: AB=` a . b .           // Show A,B bits
` State=` d .             // Show current state
` Prev=` g .              // Show previous state
` Count=` e .             // Show encoder count
` Angle=` e 15 * t!       // Calculate angle in 0.001 degree units
t 1000 / .                // Show whole degrees
`.`                       // Decimal point
t 1000 % " 100 / .        // Show first decimal place
t 100 % 10 / .            // Show second decimal place
`°` /N                    // Degree symbol and newline
;

// Function S: Main simulation loop
:S
/U(                       // Start unlimited loop
    T                     // Get current state
    U                     // Update counter
    V                     // Display results
    r 1 + r!              // Next pattern index
    r 20 < /W             // Continue while r < 20
    500()                 // Delay for readability
)
;

// Start simulation by typing: S

//////////////////////////
notes for above code

Looking at this MINT code for a quadrature encoder simulator, I can identify several syntax issues that need to be corrected according to the MINT manual:## Issues Found and Corrections Made:

### 1. **Function V - Angle Calculation**
**Original problematic line:**
```mint
` Angle=` e 15 * " 1000 / . `.`  // Show angle whole part
e 15 * 1000 % dup 100 / .       // Show angle decimal part
```

**Issues:**
- `"` (duplicate) followed by space and `1000` creates syntax confusion
- `dup` is not a MINT operator (MINT uses `"` for duplication)
- Logic for decimal display was overly complex

**Corrected approach:**
```mint
` Angle=` e 15 * t!       // Calculate angle in 0.001 degree units
t 1000 / .                // Show whole degrees
`.`                       // Decimal point
t 1000 % " 100 / .        // Show first decimal place
t 100 % 10 / .            // Show second decimal place
```

### 2. **Function S - Loop Structure**
**Original problematic code:**
```mint
/T i!                     // Initialize loop control
/U(i /W                   // Start unlimited loop
    // ...
    r 20 = (/F i!)       // Stop after 20 steps
)
```

**Issues:**
- Incorrect loop syntax - `/U(i /W` doesn't follow MINT patterns
- Complex condition handling with `i` variable unnecessary
- `r 20 = (/F i!)` syntax is not standard MINT

**Corrected approach:**
```mint
/U(                       // Start unlimited loop
    T                     // Get current state
    U                     // Update counter  
    V                     // Display results
    r 1 + r!              // Next pattern index
    r 20 < /W             // Continue while r < 20
    500()                 // Delay for readability
)
```

### 3. **General Improvements:**
- **Cleaner variable usage**: Eliminated unnecessary `i` variable
- **Proper MINT syntax**: Used correct loop termination with `/W`
- **Better readability**: Separated operations for clarity
- **Maintained functionality**: All original features preserved

### 4. **Key MINT Rules Applied:**
- Comments on separate lines (good practice)
- No space between `:` and function letter
- Proper use of `/U( ... /W )` for unlimited loops
- Correct use of `/E` for if-then-else
- Stack-based operations in correct order

The corrected code should run properly in MINT and simulate the quadrature encoder as intended, showing step-by-step state changes, encoder counts, and calculated angles.

//////////////////////////////////////////////////////////////////////////////////////



// last version of code



// 10:1 Geared Quadrature Encoder Simulator
// Tests 600 pulse/rev encoder with CW/CCW motion
// First 11 steps clockwise, next 9 counter-clockwise
// Resolution: 0.015 degrees per count
// (360° / (600 pulses × 4 quad × 10 gear ratio))

// Test pattern array:
// CW sequence:  00->01->11->10 (0,1,3,2)
// CCW sequence: 11->01->00->10 (3,1,0,2)
[0 1 3 2 0 1 3 2 0 1 3 1 0 2 3 1 0 2 3 1]p! 

// Initialize variables
0 r!  // Pattern array index
0 e!  // Encoder count value
0 g!  // Previous state storage

// Function T: Get current state and extract A,B bits
:T 
r p ? d!       // Get state from pattern array into d
d 1 & b!       // Extract B bit (LSB)
d 2 / 1 & a!   // Extract A bit (MSB)
;

// Function U: Update counter based on direction
:U 
r 11 < (       // First 11 steps = Clockwise
    e 1 + e!   // Increment counter
) /E (         // Last 9 steps = Counter-clockwise
    e 1 - e!   // Decrement counter
)
d g!           // Store current state for next comparison
;

// Function V: Display formatted output
:V
`Step ` r 1 + .           // Show step number (1-based)
`: AB=` a . b .           // Show A,B bits
` State=` d .             // Show current state
` Prev=` g .              // Show previous state
` Count=` e .             // Show encoder count
` Angle=` e 15 * " 1000 / . `.`  // Show angle whole part
e 15 * 1000 % dup 100 / .       // Show angle decimal part
`00` /N                         // Add newline
;

// Function S: Main simulation loop
:S
/T i!                     // Initialize loop control
/U(i /W                   // Start unlimited loop
    T U V                 // Process one step
    r 1 + r!             // Next pattern value
    r 20 = (/F i!)       // Stop after 20 steps
    500()                // Delay for readability
)
;

// Start simulation by typing S
S
