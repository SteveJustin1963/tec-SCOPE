Make sure you join a club if you use a laser pointer >1mW for star aiming
- https://www.police.nsw.gov.au/__data/assets/pdf_file/0008/139895/List_of_Astronomical_Societies.pdf

Nothing works yet! its a work in progress, not finshed and tested


## Let your SCC or Tec-1 control your telescope
- get urself a cheap 8-inch F5 Dobson or make one, the pushed by hand truss frame type. 
- next get ur old tec-1 out blow the dust off, add an extra 2k ram total 4k and as well as my expansion board
- mod the mount with 2 motors and 2 encoders
- get Stellarium put on a laptop with a serial port u can use usb to serial, ull meed 2 serial ports
- play

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)

 

### mount
- alt (up down) and azimuth (side to side) lets say x and y, are friction coupled, convert this to gears and bearing support 
- we need to add 2 motors and 2 encoders
- where to get a free motor with a reduction gear? hack one out of a thrown out foot massager, its motor is 24v and has a worm drive shaft coming out each side
- for the az motor need somthing else as we have no rooom under the mount
- 

 
![image](https://github.com/user-attachments/assets/f66e2597-9af3-4984-bb4c-550e19ec3681)



### encoders
  - HN3806-AB (around $15), has AB 2 phase rectangular orthogonal pulses 600 pulses/rev aka 2 wires, max 5000rpm we are not going that fast, response 0-20KHz 
  - green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
  - do not connect AB directly VCC!! go to the chip 

 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)

### SPI

![image](https://github.com/user-attachments/assets/c596d7a4-f89f-4522-9ba0-565d6488245d)

 

will try and bring spi, LS7366R and 9511 into one system.


![Schematic_tec-Scope_2024-10-28](https://github.com/user-attachments/assets/85bd0e3b-1a09-4705-82ca-f70607eb217d)


 





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
- expenisve chip! over $30
- try a ATtiny85 with Quadrature Encoder Implementation for $1

### rotary encoder
The **A** and **B** signals from the rotary encoder are part of a **quadrature output**. 
This means the encoder uses the two signals (A and B) that are 90 degrees out of phase to determine both the **position** and **direction** of movement. 

Here's how it works:
1. **A and B Signals**: 
   - The encoder produces two square wave signals (A and B). 
   - These signals are offset by 90 degrees (a quarter of a cycle). This phase difference is what allows the system to determine the direction of rotation.

2. **Counting Pulses**:
   - As the encoder rotates, these signals will alternate between HIGH (1) and LOW (0).
   - If you count the transitions (rising or falling edges) on each channel, you can determine the movement's distance or angle.

3. **Determining Direction**:
   - By checking the sequence in which A and B change states, you can tell if the movement is clockwise or counterclockwise:
     - If **A** leads **B** (A changes state before B), it indicates movement in one direction (e.g., clockwise).
     - If **B** leads **A**, it indicates movement in the opposite direction (e.g., counterclockwise).

When connected to the LS7366R:
- The LS7366R uses these signals to increment or decrement the counter based on the direction and the number of pulses detected.
- It supports different counting modes (e.g., 1X, 2X, 4X) to capture pulses for higher resolution, using both edges of A and B.

### control the LS7366R 

- assumning we ignore the **INDEX/** pin 
- and set up the counter for counting (either initializing it with a specific count or clearing it), 
- we need to configure **MDR0** and **MDR1** accordingly.
- step-by-step setup:

### 1. **Configure MDR0** (Mode Register 0):
This register sets up the counting mode, quadrature type, and index behavior.

- **Bits**:  
  - **B7**: Filter clock division (set as needed; `0` for division factor 1).  
  - **B6**: Synchronous/Asynchronous index (set to `0` if ignoring the index).  
  - **B5:B4**: Index function (set to `00` to disable the index).  
  - **B3:B2**: Counting mode (e.g., `00` for free-running mode).  
  - **B1:B0**: Quadrature mode (e.g., `11` for x4 quadrature).

Here’s an example configuration for **MDR0**:
- **MDR0** = `0x03`:
  - `00`: Disable index (`B5:B4`).
  - `00`: Free-running count mode (`B3:B2`).
  - `11`: x4 quadrature mode (`B1:B0`).

**Hex Value for MDR0**: `0x03`

### 2. **Configure MDR1** (Mode Register 1):
This register sets the data width and flag options.

- **Bits**:  
  - **B7:B6**: Enable flags like Carry (CY), Borrow (BW), Compare (CMP), etc.  
  - **B3**: Not used.  
  - **B2**: Counting enable/disable (set to `0` to enable counting).  
  - **B1:B0**: Counter byte width (`00` for 4 bytes, `01` for 3 bytes, etc.).

Here’s an example configuration for **MDR1**:
- **MDR1** = `0x00`:
  - `00`: 4-byte counter mode (`B1:B0`).
  - `0`: Enable counting (`B2`).
  - `0000`: No flags enabled (`B7:B6`).

**Hex Value for MDR1**: `0x00`

### 3. **Load the Counter (CNTR) or Clear It**:
   - To **clear** the counter: Send the **CLR CNTR** command (`0x20`).
   - To **load** the counter with a specific value, do the following:
     1. Write the desired value to the **DTR** register using the **WRITE DTR** command (`0x98`), followed by the value bytes (up to 4 bytes depending on your configuration).
     2. Send the **LOAD CNTR** command (`0xE0`) to transfer the value from **DTR** to **CNTR**.

### Summary Commands:
- **MDR0**: `0x03` (Disable index, x4 quadrature, free-running).
- **MDR1**: `0x00` (4-byte counter, counting enabled, no flags).

 

## ATtiny84 Quadrature Encoder Implementation
am already sick of the LS7366R, lets do it on the ATtiny84 and 85


////////////



### az with magnetic sensor
we shud add a KMA215 
- accurate for angle measurement
- Resolution to 0.1 degrees
- angle accuracy ±1 degree
- depends on:
   - The strength and stability of the magnetic field.
   - The environmental conditions, such as temperature variations.
   - Proper alignment and calibration of the sensor with the magnet.
- operate accurately across a wide temperature range

### sudo KMA215 SPI 

- Initialize SPI communication settings specific to the KMA215.
- ReadAngle
   - Pulls the chip select (CS) low to initiate communication.
   - Sends a command (e.g., `0xA0`) to request the angle data.
   - Reads two bytes (MSB and LSB) from the sensor.
   - Combines the bytes into a 16-bit value and converts it into degrees.
- Main Loop, continuously reads and prints the angle.


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




### code loop ideas 
- we will ise a simple  x y joystick slew it around with the dc motors
- and also via via tec1 commands
- or track a position using an optical gates or estimation 
- will try starmap Stellarium interface via another bitbang port x.200 commands
- what to do for long-exposure astrophotography ?

#### work to be done 
- read encoders x2 with interrupt start
- motor control x2
- display data
  - read data from encoders, convert to angle, display on tec1s seven segment display of 6 digits xxxx xx
  - press A gives the azimuth and alt together to 1 degree accuracy, eg az= 0299 alt = 23 displayed as 0299 23
  - press B then only az is displayed deg-min eg 0299 44 meaning 299 degs 44 minutes
  - press C then only alt is displayed deg-min eg 0023 51 meaning 23 deg 51 min
  - no need for precision in seconds unless the machanicals of the telescope can justify it  
- Stellarium integration
  - /www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/

- calibration
  - angle check using iphone for compass,
    - it has a level guide in it as well with the little cross in the middle. adjust thumb screw in base
    - true north in Sydney add 12.83°E in 2020 changes 0.04° annually
  - a plastic protractor and plumb for angle
  - once positioned right then calibrate with alignment to a known celestial object

 

### scope-1.mint 

Code Summary

1. **SPI Configuration and Operation**:
   - The code initializes the LS7366R using SPI communication, configures the counting mode (x4 quadrature, free-running), clears or loads the counter, and retrieves a 32-bit count value.

2. **FPU (AM9511A) Configuration and Operation**:
   - The FPU is set up with basic functions for initializing, storing arguments, performing addition, checking for errors, and retrieving results. This allows calculations within the MINT program, handling floating-point arithmetic where needed.

3. **Main Program Flow**:
   - The `:START` routine initializes the LS7366R, clears the counter, initializes FPU arguments, performs a simple operation (addition), checks for errors, and reads back the results. The program then enters an infinite loop awaiting further input or commands.

### Testing and Validation Tips

- **Verify Port Addresses**: Ensure that all defined ports (e.g., `DATA_PORT`, `COMMAND_PORT`, `STATUS_PORT`, `SPI_CS_PORT`, etc.) align with the actual connections in your TEC-SCOPE hardware.
- **Timing**: Adjust delays if necessary for SPI reliability and FPU response.
- **Testing Routine**: Test each section independently before running the complete program to validate SPI communication, FPU operations, and data integrity.
 







