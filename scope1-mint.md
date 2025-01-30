ver1

```
// Port Constants
10 p! // MISO/DATA port
11 q! // MOSI/CMD port
12 r! // CLK/STATUS port
13 s! // CS port

// LS7366R Commands
#88 t! // Write MDR0
#90 u! // Write MDR1
#20 v! // Clear counter
#98 w! // Write DTR
#E0 x! // Load counter
#60 y! // Read counter

// FPU Commands
#6C z! // Add command
#80 a! // Busy flag
#1E b! // Error mask

// SPI Functions
:A 1 r! ; // Clock high
:B 0 r! ; // Clock low
:C 0 s! ; // CS enable
:D 1 s! ; // CS disable

// Send byte via SPI
:E i! // Store byte
C    // Enable CS
8(   // 8-bit loop
  i #80 & ( 1 q! ) /E ( 0 q! )
  A B   // Clock pulse
  i { i!  // Shift left
)
D    // Disable CS
;

// Read byte via SPI
:F 0 j! // Clear result
C      // Enable CS
8(     // 8-bit loop
  A B   // Clock pulse
  p /I 1 & ( j { 1 + j! ) /E ( j { j! )
)
D      // Disable CS
j      // Return result
;

// Initialize LS7366R
:G 
t #03 E // MDR0 config
u 0 E   // MDR1 config
;

// Clear counter
:H v E ;

// Load counter value
:I k! // Store value
w k #FF000000 & #18 } E
w k #FF0000 & #10 } E
w k #FF00 & #8 } E
w k #FF & E
x E
;

// Read counter
:J
y E    // Send read command
0 l!   // Clear result
F #18 { l +! // Read and shift bytes
F #10 { l +!
F #8 { l +!
F l +!
l      // Return result
;

// FPU Functions
:K 1 m! 1 n! ; // Init args

:L // Store in FPU
m p /O
m } q /O
n p /O
n } q /O
;

:M z q /O ; // Add command

:N // Wait for FPU
/U ( r /I a & /W )
;

:O // Check errors
r /I b & { o!
;

:P // Pop result
p /I h!
p /I i!
i #8 { h + j!
;

// Main Program
:S
G     // Init LS7366R
H     // Clear counter
K     // Init FPU
J     // Read counter
L     // Store in FPU
M     // Add
N     // Wait
O     // Check errors
P     // Get result
/U ( /K ) // Loop for input
;
```

# ver2 31.1.25
```
 // Port Constants
10 p!   // MISO/DATA port
11 q!   // MOSI/CMD port
12 r!   // CLK/STATUS port
13 s!   // CS port

// LS7366R Command Constants
#88 t!  // Write MDR0 command
#90 u!  // Write MDR1 command
#20 v!  // Clear counter command
#98 w!  // Write DTR command
#E0 x!  // Load counter command
#60 y!  // Read counter command

// FPU Command Constants
#6C z!  // Add command
#80 a!  // Busy flag
#1E b!  // Error mask

// Basic SPI Control Functions
:A 1 r! ;  // Clock high
:B 0 r! ;  // Clock low
:C 0 s! ;  // CS enable
:D 1 s! ;  // CS disable

// SPI Data Transfer Functions
:E i!      // Send byte via SPI
  C        // Enable CS
  8(       // 8-bit loop
    i #80 & ( 1 q! ) /E ( 0 q! )  // Set MOSI based on MSB
    A B    // Clock pulse
    i { i! // Shift left
  )
  D        // Disable CS
;

:F          // Read byte via SPI
  0 j!      // Clear result
  C         // Enable CS
  8(        // 8-bit loop
    A B     // Clock pulse
    p /I 1 & ( j { 1 + j! ) /E ( j { j! )  // Read MISO
  )
  D         // Disable CS
  j         // Return result
;

// LS7366R Functions
:G          // Initialize LS7366R
  t #03 E   // Configure MDR0
  u 0 E     // Configure MDR1
;

:H v E ;    // Clear counter

:I k!       // Load counter value
  w k #FF000000 & #18 } E  // Load highest byte
  w k #FF0000 & #10 } E    // Load high byte
  w k #FF00 & #8 } E       // Load low byte
  w k #FF & E              // Load lowest byte
  x E                      // Send load command
;

:J          // Read counter
  y E       // Send read command
  0 l!      // Clear result
  F #18 { l +!  // Read highest byte
  F #10 { l +!  // Read high byte
  F #8 { l +!   // Read low byte
  F l +!        // Read lowest byte
  l             // Return result
;

// FPU Functions
:K 1 m! 1 n! ;  // Initialize arguments

:L              // Store in FPU
  m p /O        // Store first argument high byte
  m } q /O      // Store first argument low byte
  n p /O        // Store second argument high byte
  n } q /O      // Store second argument low byte
;

:M z q /O ;     // Send add command

:N              // Wait for FPU to complete
  /U ( r /I a & /W )
;

:O              // Check for errors
  r /I b & { o!
;

:P              // Pop result from FPU
  p /I h!
  p /I i!
  i #8 { h + j!
;

// Main Program
:S
  G            // Initialize LS7366R
  H            // Clear counter
  K            // Initialize FPU
  /T t!        // Set initial loop condition to true
  /U (         // Start unlimited loop
    t /W       // Continue while t is true
    J          // Read counter
    L          // Store in FPU
    M          // Add
    N          // Wait for completion
    O          // Check for errors
    P          // Get result
    /K 27 = (  // Check if ESC key (27) was pressed
      /F t!    // If ESC, set t to false to break loop
    )
  )
;
```
