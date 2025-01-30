
// Stage 1: Test Port Setup
// Test if we can set ports correctly
:A
10 p! // MISO/DATA port
11 q! // MOSI/CMD port
12 r! // CLK/STATUS port
13 s! // CS port
p . /N q . /N r . /N s .  // Should print 10 11 12 13
;

// Stage 2: Test Basic SPI Control
// Test if SPI control signals work
:B
1 r! // Clock high
0 r! // Clock low
0 s! // CS enable
1 s! // CS disable
;

// Stage 3: Test SPI Byte Transfer
// Test sending a single byte via SPI
:C i! // Send byte via SPI
0 s! // CS enable
8(   // 8-bit loop
  i #80 & ( 1 q! ) /E ( 0 q! )
  1 r! 0 r!   // Clock pulse
  i { i!  // Shift left
)
1 s! // CS disable
;

// Stage 4: Test SPI Byte Read
// Test reading a single byte via SPI
:D 
0 j! // Clear result
0 s! // CS enable
8(   // 8-bit loop
  1 r! 0 r!   // Clock pulse
  p /I 1 & ( j { 1 + j! ) /E ( j { j! )
)
1 s! // CS disable
j .  // Print result
;

// Stage 5: Test LS7366R Basic Commands
// Test if we can send commands to counter
:E 
#88 t! // Write MDR0 command
#90 u! // Write MDR1 command
t C    // Send MDR0 command
#03 C  // Send config
u C    // Send MDR1 command
0 C    // Send config
;

// Stage 6: Test Counter Operations
// Test reading/writing counter
:F
#20 C    // Clear counter command
#60 C    // Read counter command
D        // Read byte
;

// Stage 7: Test FPU Basic Operations
// Test FPU communication
:G
1 m! 1 n!     // Set test values
m p /O        // Send to FPU
m } q /O
n p /O
n } q /O
#6C q /O      // Add command
;

// Stage 8: Test Complete Loop
// Test main loop with proper exit
:H
/T t!        // Set loop control
/U (         // Start unlimited loop
  t /W       // Continue while t is true
  `Press ESC to exit` /N
  /K 27 = (  // Check if ESC key pressed
    /F t!    // Set false to break loop
  )
)
;

