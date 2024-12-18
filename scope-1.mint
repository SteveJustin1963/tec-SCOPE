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

