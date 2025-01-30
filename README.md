under development, not ready


# aiming
- get or make a cheap 8-inch F5 Dobson, truss or tube, pushed by hand
- red dot aiming device needs neck bending
- to save ur neck use a Sky-Watcher 6x30mm Right Angled (90deg angle viewer) Erect Image (shows objects right-side up and not inverted or reversed) Finder Scope. Ideal for fast, intuitive location of target objects. 
- laser pointer >1mW for star aiming u need an astronomy permit https://www.police.nsw.gov.au/__data/assets/pdf_file/0008/139895/List_of_Astronomical_Societies.pdf


# control your telescope
- tec1 or scc using MINT
- add to the mount geared down 2 rotary encoders and dc motors 
- later we get code to send via 4800 serial async to Stellarium so it can correlate with star map 
- 

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)

 

# mount
- alt (up down) and azimuth (side to side) lets say x and y, are friction coupled, convert this to gears and bearing support 
- we need to add 2 motors and 2 encoders
- for an alt motor with a reduction worm drive with perpendicular shafts on each side can be got from a discarded foot massage machine, it has a 24v motor, need to be run at a lower voltage and pwm to slow it down as its powerful and fast, fast is not what we want. 
- for the az motor need something else as we have no room under the mount
- 

 
![image](https://github.com/user-attachments/assets/f66e2597-9af3-4984-bb4c-550e19ec3681)




#### rotary encoder
- HN3806-AB (around $15), has AB 2 phase rectangular orthogonal pulses 600 pulses/rev aka 2 wires, max 5000rpm we are not going that fast, response 0-20KHz 
- green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
- do not connect AB directly VCC!! go to the chip **Use pull resistors**
- A and B from the rotary encoder are part of a quadrature output.
- means the encoder uses the two signals (A and B) that are 90 degrees out of phase to determine position and direction of movement. 
- we need to counting the pulses, signals alternate between HIGH (1) and LOW (0).
- count the transitions (rising or falling edges) on each channel, we can determine the movement's distance or angle.
- If A leads B eg clockwise
- If B leads A eg counterclockwise
    

 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)

```
// Port assignments for encoders
2 a! // Encoder 1 - A pulse
3 b! // Encoder 1 - B pulse
4 c! // Encoder 2 - A pulse
5 d! // Encoder 2 - B pulse

// Variables for counting
0 e! // Encoder 1 count
0 f! // Encoder 2 count

// Previous quadrature states
0 g! // Previous A1,B1 state
0 h! // Previous A2,B2 state

// Constants for angle calculation
// 600 pulses × 4 (quadrature) × 10 (gear) = 24000 pulses per mechanical revolution
// 360 degrees / 24000 = 0.015 degrees per pulse
// Multiply by 1000 for 3 decimal places: 0.015 × 1000 = 15
15 j! // Degrees per 1000 pulses

// Quadrature state lookup table for clockwise rotation
// [00->01->11->10->00]
// Returns 1 for clockwise, -1 for counter-clockwise, 0 for invalid
:Q k!
// Input: old state in high nibble, new state in low nibble
  k #0F &    // Mask new state
  k 4 } #0F & // Shift and mask old state
  2 * +      // Combine states to use as index
  [0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0] $! // Store lookup table
  $ k ? .    // Return direction from lookup
;

// Function to read encoder 1
:A 
  a /I 2 * b /I +  // Get current state (combine A and B)
  g 4 * +         // Combine with previous state
  " Q             // Get direction from lookup
  " 0 = /F (      // If valid transition
    e $ + e!      // Update count
  )
  g!              // Store current state
;

// Function to read encoder 2
:B
  c /I 2 * d /I +  // Get current state (combine A and B)
  h 4 * +         // Combine with previous state
  " Q             // Get direction from lookup
  " 0 = /F (      // If valid transition
    f $ + f!      // Update count
  )
  h!              // Store current state
;

// Function to calculate degrees (with 3 decimal places)
:D k! // Input: count in k
  k j *  // Multiply count by degrees/1000
  1000 / // Divide by 1000 to get actual degrees
;

// Function to display angle with decimal point
:E k!           // Input: count in k
  k D " 1000 /  // Get whole degrees
  `.`           // Decimal point
  k D 1000 % " 100 / . // Show 3 decimal places
  ` degrees`    // Print units
;

// Main loop to read both encoders and show angles
:C
  /T i!          // Set loop control flag
  /U (           // Start unlimited loop
    i /W         // Continue while flag is true
    A B          // Read both encoders
    `Encoder 1: Count:` e . 
    ` Angle:` e E /N
    `Encoder 2: Count:` f . 
    ` Angle:` f E /N
    50()         // Small delay (reduced for better response)
    /K 27 = (    // Check for ESC key
      /F i!      // Set flag false to exit
    )
  )
;

// Function to reset counters to zero
:F 
  0 e! 0 f!     // Reset both counts to zero
  `Counters reset to zero` /N
;
```

## test with dummy data loop
```
// Initialize quadrature state array
// States: 00,01,11,10 representing AB bits
// 0 = 00 (A=0,B=0)
// 1 = 01 (A=0,B=1)
// 3 = 11 (A=1,B=1)
// 2 = 10 (A=1,B=0)
[0 1 3 2]p!    

// Initialize variables:
// r = array index for pattern lookup
// e = encoder count value
// g = previous state storage
0 r! 0 e! 0 g!  

// Function T: Get and process current state
:T           
   // Handle array index wraparound
   r 3 > ( 0 r! )     

   // Get pattern value at current index
   p r ? a!           
   `Raw value:` a . /N

   // Extract B bit using AND with 1
   // This keeps only LSB
   a 1 & b!           

   // Extract A bit by first shifting right (/2)
   // then AND with 1 to get clean bit
   a 2 / 1 & c!       

   // Display current AB bits
   `AB:` c . b . /N   

   // Combine A,B into state number
   // State = A*2 + B
   c 2 * b + d!       

   // Show current and previous states
   `State:` d . ` Prev:` g . /N  
;

// Function U: Update counter based on state transitions
:U              
   // Calculate state transition difference
   // This tells us direction of movement
   d g - " n!   

   // Check for clockwise transitions
   // +1 difference indicates CW movement (except at wrap)
   n 1 = (      
      e 1 + e!  // Increment counter
   )

   // Handle clockwise wraparound from state 2->0
   // Appears as -3 difference
   n -3 = (     
      e 1 + e!  // Increment counter
   )

   // Check for counter-clockwise transitions
   // -1 difference indicates CCW movement (except at wrap)
   n -1 = (     
      e 1 - e!  // Decrement counter
   )

   // Handle counter-clockwise wraparound from state 0->2
   // Appears as +3 difference
   n 3 = (      
      e 1 - e!  // Decrement counter
   )

   // Store current state for next comparison
   d g!         

   // Display current count
   `Count:` e . /N  
;

// Function V: Calculate and display angle
:V              
   // Scale factor: 0.015 degrees per count
   // (based on 600 pulses/rev × 4 quad × 10:1 gear)
   e 15 *       

   // Display whole degrees
   " 100 / .    

   // Decimal point
   `.`          

   // Calculate and display fraction
   100 % 10 / . 

   // Add units and newline
   ` deg` /N    
;

// Function S: Main test loop
:S              
   // Initialize loop control flag
   /T i!        

   // Start unlimited loop
   /U (         
      // Continue while flag is true
      i /W      

      // Visual separator
      `---` /N  

      // Process one complete cycle:
      // Get state, update count, show angle
      T U V     

      // Move to next pattern value
      r 1 + r!  

      // Delay for visibility
      500()     

      // Check for ESC key to exit
      /K 27 = ( 
         /F i!  
      )
   )
;

// Start the test by typing S

S
```



### SPI
 
![image](https://github.com/user-attachments/assets/c596d7a4-f89f-4522-9ba0-565d6488245d)

see http://easyeda.com for schematics, links coming 


### counter chip LS7366R
LS7366R runs on SPI and is a quadrature encoder counter IC designed to interface with rotary or linear encoders. 
It can track position, direction, and speed by counting the pulses from the encoder. 
The LS7366R can operate in various modes, including 1X, 2X, or 4X decoding, to count the pulses per encoder cycle, 
and it supports a 32-bit counter for high precision.

Key features include:
- SPI interface for communication with microcontrollers or processors.
- Quadrature decoding for up to four times the encoder resolution.
- Support for various count modes (up, down, quadrature, and modulo).
- A 32-bit counter register that allows for high-resolution tracking.
- expensive chip! over $30
- i want the 9511 mpu as i think we may have to do some hard computing on celestial computing
 
![Schematic_tec-Scope_2024-11-01 (1)](https://github.com/user-attachments/assets/54d99106-fb3e-41d8-b811-f4e2620b6e7e)
![image](https://github.com/user-attachments/assets/466d03ed-0395-4116-8731-8d275b041857)
![image](https://github.com/user-attachments/assets/0f5bb19e-24f4-4d7f-96f7-87e9281c3b14)




#### LS7366R_9511_ SPI.mint

	
### Code Summary

1. **SPI Configuration and Operation**:
   - The code initialises the LS7366R using SPI communication, configures the counting mode (x4 quadrature, free-running), clears or loads the counter, and retrieves a 32-bit count value.

2. **FPU (AM9511A) Configuration and Operation**:
   - The FPU is set up with basic functions for initialising, storing arguments, performing addition, checking for errors, and retrieving results. This allows calculations within the MINT program, handling floating-point arithmetic where needed.

3. **Main Program Flow**:
   - The `:START` routine initialises the LS7366R, clears the counter, initialises FPU arguments, performs a simple operation (addition), checks for errors, and reads back the results. The program then enters an infinite loop awaiting further input or commands.

4. **Testing and Validation**:  
   - **Verify Port Addresses**: Ensure that all defined ports (e.g., `DATA_PORT`, `COMMAND_PORT`, `STATUS_PORT`, `SPI_CS_PORT`, etc.) align with the actual connections in your TEC-SCOPE hardware.
   - **Timing**: Adjust delays if necessary for SPI reliability and FPU response.
   - **Testing Routine**: Test each section independently before running the complete program to validate SPI communication, FPU operations, and data integrity.





 

#### ATtiny84/85 Quadrature Encoder Implementation
- lets try the ATtiny85 with Quadrature Encoder Implementation for $1, or the 84 if we need more gpios
- am already sick of the LS7366R due to the complexity and the high cost and hard to find it, lets do it with two attiny85 or one attiny84.
- we can just modify the exiting code and  setup using two ATtiny85s, where each chip handles one encoder and communicates as SPI slave.


Key features of this two-chip solution:

1. **Pin Assignments** (per schematic):
   - SPI_CLK on PB3 (pin 2)
   - SPI_MISO on PB4 (pin 3)
   - Encoder A on PB0 (pin 5)
   - Encoder B on PB1 (pin 6)
   - Chip Select on PB2 (pin 7)
   - RESET on PB5 (pin 1)

2. **Advantages of This Setup**:
   - No pin sharing conflicts
   - Each chip dedicated to one encoder
   - Cleaner interrupt handling
   - No collision detection needed
   - Independent operation of each encoder

3. **Compilation**:
   - Define `CHIP_1` for the first ATtiny85 (U2)
   - Comment out `CHIP_1` for the second ATtiny85 (U5)

4. **SPI Communication**:
   - Each chip responds only when its CS line is LOW
   - Command 0x01: Read encoder count
   - Command 0x02: Read status information

To use this system:

1. **Compile Two Versions**:
   - One with `CHIP_1` defined (for U2)
   - One with `CHIP_1` commented out (for U5)

2. **Program Each Chip**:
   - Program U2 with the first version
   - Program U5 with the second version

3. **Read Values**:
   - Select appropriate chip with CS line
   - Send command (0x01 or 0x02)
   - Read 4 bytes for counter or 3 bytes for status


![image](https://github.com/user-attachments/assets/2b6cd303-1f15-4b7e-a85e-055055a94236)

![image](https://github.com/user-attachments/assets/cb468bee-9abd-48b9-9c95-75f166703ad7)
![image](https://github.com/user-attachments/assets/3cc8b127-2a64-4820-9200-6a2e945ad0da)



- if we add more spi slaves as we will I think ill migrate to ardino, stay tuned
- 

### KMA215 for az reading 
- a magnetic sensor; the KMA215 for angle measurements
- it has an resolution to 0.1 degrees, accuracy ±1 degree
- effected by strength and stability of the magnetic field.
- we can check it against our calibrated estimation
- chosen because it has SPI 
- to ReadAngle
  - pull the chip select (CS) low to initiate communication.
  - we sends a command (e.g., `0xA0`) to request the angle data.
  - we reads two bytes (MSB and LSB) from the sensor.
  - we combines the bytes into a 16-bit value and converts it into degrees
  - we can use it, display it, etc 

we could do some pre processing on the attiny85 or do it from mint code

```
// Pseudocode for reading angle data from KMA215 using SPI

// Initialize SPI communication
InitializeSPI()
{
    Set SPI mode to Mode 0 (CPOL = 0, CPHA = 0)
    Set SPI clock speed (e.g., 1 MHz)
    Set data order to MSB first
}

// Function to read angle data
ReadAngle()
{
    // Select the KMA215 (pull CS pin low)
    SetChipSelectLow()
    
    // Send command to read angle data (example command: 0xA0)
    SendSPIByte(0xA0)
    
    // Read the first byte (most significant byte)
    MSB = ReadSPIByte()
    
    // Read the second byte (least significant byte)
    LSB = ReadSPIByte()
    
    // Deselect the KMA215 (pull CS pin high)
    SetChipSelectHigh()
    
    // Combine the MSB and LSB into a 16-bit value
    RawAngle = (MSB << 8) | LSB
    
    // Convert raw angle value to degrees (example conversion)
    AngleInDegrees = (RawAngle / 65536.0) * 360.0
    
    // Return the calculated angle
    Return AngleInDegrees
}

// Main routine
Main()
{
    // Initialize SPI
    InitializeSPI()
    
    // Continuously read and display the angle
    While (true)
    {
        Angle = ReadAngle()
        Print("Angle: ", Angle, " degrees")
        
        // Add delay or other processing as needed
        Delay(100 ms)
    }
}
```




#### main code loop
- build up simple features first
- motor control /manual or code / / slew joystick- dc motors
- track a position using an optical gates or estimation on Stellarium
- star map Stellarium interface via another bit bang port x.200 commands
- long-exposure astro-photography

- read encoders, convert to angle, display on  seven segment display of 6 digits xxxx xx
  - press A gives the azimuth and alt together to 1 degree accuracy, eg az= 0299 alt = 23 displayed as 0299 23
  - press B then only az is displayed deg-min eg 0299 44 meaning 299 degs 44 minutes
  - press C then only alt is displayed deg-min eg 0023 51 meaning 23 deg 51 min
  - no need for precision in seconds unless the mechanical of the telescope can justify it  



- Stellarium integration
  - /www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/

- calibration
  - az check, true north in Sydney add 12.83°E in 2020 changes 0.04° annually
  - level
  - calibrate with known celestial object


 



### generic SPI code

generic_SPI.z80


1. **Generic `spi_write` Routine**:
   - This routine can be used to send any 8-bit command (register or operation) and
 8-bit data to an SPI device by setting `D` and `E` with the required values.
   
2. **Buffer Send Routine (`spi_send_data`)**:
   - This subroutine allows sending multiple bytes from a buffer, which is useful for devices needing continuous data transmission.
   - By setting `HL` to point to the buffer and `D` to the number of bytes, we can send any arbitrary amount of data to the SPI device.

3. **Device-Agnostic Initialisation**:
   - The initialisation routine (`init_spi`) simply sets the SPI bus to an idle state, without device-specific configurations.
   
4. **Configuration Outside Code**:
   - Device-specific configurations can be handled in calling code by setting registers (`D` and `E`) before calling `spi_write`, allowing reuse of this code across multiple SPI devices.



 # playing with code

![image](https://github.com/user-attachments/assets/48cf9d0e-ecba-4236-a271-55e19285d0e3)


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



```
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

// Usage instructions for testing:
// 1. First test port setup:
//    > A
//    Should see: 10 11 12 13
//
// 2. Test SPI control:
//    > B
//    Use oscilloscope/logic analyzer to verify signals
//
// 3. Test byte send:
//    > #AA C
//    Verify on scope/analyzer
//
// 4. Test byte read:
//    > D
//    Should read and display a byte
//
// 5. Test LS7366R setup:
//    > E
//    Verify commands on scope/analyzer
//
// 6. Test counter:
//    > F
//    Should show counter value
//
// 7. Test FPU:
//    > G
//    Verify communication on scope/analyzer
//
// 8. Test loop control:
//    > H
//    Should loop until ESC pressed

```






