
# warning  
- messy, changing and unfinished 
- make sure you join a club if you use a laser pointer >1mW for star aiming
- https://www.police.nsw.gov.au/__data/assets/pdf_file/0008/139895/List_of_Astronomical_Societies.pdf


## Let your SCC or Tec1 control your telescope
- get urself a cheap 8-inch F5 Dobson or make one, the pushed by hand truss frame type.
- or make a mock up with cardboard and mount a dslr at the base, have x y or alt, az ability
- the mount needs 2 rotary encoders
- later stellarium can talk via a async serial port the scc/tec1
- 

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




#### rotary encoder
- HN3806-AB (around $15), has AB 2 phase rectangular orthogonal pulses 600 pulses/rev aka 2 wires, max 5000rpm we are not going that fast, response 0-20KHz 
- green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
- do not connect AB directly VCC!! go to the chip Use pull resistors
- A and B from the rotary encoder are part of a quadrature output.
- means the encoder uses the two signals (A and B) that are 90 degrees out of phase to determine position and direction of movement. 
- we need to counting the pulses, signals alternate between HIGH (1) and LOW (0).
- count the transitions (rising or falling edges) on each channel, we can determine the movement's distance or angle.
- If A leads B eg clockwise
- If B leads A eg counterclockwise
    

 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)

### SPI
 
![image](https://github.com/user-attachments/assets/c596d7a4-f89f-4522-9ba0-565d6488245d)

see easyeda for schematic 


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

![image](https://github.com/user-attachments/assets/b962b0fd-e9ed-4915-aeaa-81f22fcee402)
![image](https://github.com/user-attachments/assets/e2a43ad6-063b-4000-9fa8-8a5cf43e280c)
![image](https://github.com/user-attachments/assets/3a871758-9b76-4510-877a-0d354a93b3db)

#### LS7366R_9511_ SPI.mint

	
### Code Summary

1. **SPI Configuration and Operation**:
   - The code initializes the LS7366R using SPI communication, configures the counting mode (x4 quadrature, free-running), clears or loads the counter, and retrieves a 32-bit count value.

2. **FPU (AM9511A) Configuration and Operation**:
   - The FPU is set up with basic functions for initializing, storing arguments, performing addition, checking for errors, and retrieving results. This allows calculations within the MINT program, handling floating-point arithmetic where needed.

3. **Main Program Flow**:
   - The `:START` routine initializes the LS7366R, clears the counter, initializes FPU arguments, performs a simple operation (addition), checks for errors, and reads back the results. The program then enters an infinite loop awaiting further input or commands.

4. **Testing and Validation**:  
   - **Verify Port Addresses**: Ensure that all defined ports (e.g., `DATA_PORT`, `COMMAND_PORT`, `STATUS_PORT`, `SPI_CS_PORT`, etc.) align with the actual connections in your TEC-SCOPE hardware.
   - **Timing**: Adjust delays if necessary for SPI reliability and FPU response.
   - **Testing Routine**: Test each section independently before running the complete program to validate SPI communication, FPU operations, and data integrity.





 

#### ATtiny84 Quadrature Encoder Implementation
am already sick of the LS7366R, lets do it on two attiny85's

I'll modify the code for a setup using two ATtiny85s, where each chip handles one encoder and communicates via SPI. This configuration is much cleaner as it eliminates pin sharing issues.



```c
// Common definitions for both chips
#define SPI_CLK     PB3    // Pin 2
#define SPI_MISO    PB4    // Pin 3
#define ENCODER_A   PB0    // Pin 5
#define ENCODER_B   PB1    // Pin 6
// PB2 is Chip Select (unique for each chip)
// PB5 is RESET

// First ATtiny85 (U2) - Encoder 1
#define CHIP_SELECT PB2    // Pin 7 - SPI_/CS1

// Second ATtiny85 (U5) - Encoder 2
//#define CHIP_SELECT PB2    // Pin 7 - SPI_/CS2

// Choose which chip to compile for
#define CHIP_1    // Comment this out to compile for second chip

/**** Code for each ATtiny85 ****/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

// Counter limits
#define MAX_COUNT 0x7FFFFFFF
#define MIN_COUNT -0x80000000

// Debounce configuration
#define DEBOUNCE_TIME 1000  // microseconds

// Commands
#define CMD_READ_COUNT    0x01
#define CMD_READ_STATUS   0x02
#define ERR_INVALID_CMD   0xFF

// Encoder counter and state
volatile int32_t encoder_count = 0;
volatile uint8_t prev_state = 0;

// SPI state
volatile uint8_t spi_command = 0;
volatile uint8_t byte_counter = 0;

// Status tracking
volatile struct {
    uint8_t errors;           // Error counter
    uint8_t overflow_count;   // Counter overflow events
    uint8_t last_command;     // Last received command
} status = {0, 0, 0};

// Timing variables
volatile uint32_t last_interrupt_time = 0;
volatile uint32_t micros_counter = 0;

uint32_t micros(void) {
    uint32_t mc;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        mc = micros_counter;
    }
    return mc;
}

ISR(TIM0_OVF_vect) {
    micros_counter += 256;
}

void setup_timer(void) {
    TCCR0A = 0;
    TCCR0B = (1 << CS00);    // No prescaling
    TIMSK |= (1 << TOIE0);   // Enable overflow interrupt
}

void setup_spi(void) {
    // Configure USI for SPI slave operation
    DDRB |= (1 << SPI_MISO);           // MISO as output
    DDRB &= ~((1 << SPI_CLK) |         // CLK as input
              (1 << CHIP_SELECT));      // CS as input
    PORTB |= (1 << CHIP_SELECT);       // Enable pull-up on CS
    
    // USI in Three-wire (SPI) mode
    USICR = (1 << USIWM0) |            // Three-wire mode
            (1 << USICS1) |            // External clock source
            (0 << USICS0) |            // Positive edge
            (1 << USIOIE);             // Enable overflow interrupt
    
    USISR = (1 << USIOIF);             // Clear overflow flag
}

void setup_encoder(void) {
    // Configure encoder pins as inputs with pull-ups
    DDRB &= ~((1 << ENCODER_A) | (1 << ENCODER_B));
    PORTB |= ((1 << ENCODER_A) | (1 << ENCODER_B));
    
    // Enable pin change interrupts
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << ENCODER_A) | (1 << ENCODER_B);
}

// SPI interrupt handler
ISR(USI_OVF_vect) {
    // Only process if chip is selected (CS is LOW)
    if (PINB & (1 << CHIP_SELECT)) {
        return;
    }

    uint8_t spi_data = USIDR;
    int32_t count_value;
    
    if (byte_counter == 0) {
        status.last_command = spi_data;
        
        switch(spi_data) {
            case CMD_READ_COUNT:
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    count_value = encoder_count;
                }
                USIDR = (count_value >> 24) & 0xFF;  // Send MSB
                byte_counter = 1;
                break;
                
            case CMD_READ_STATUS:
                USIDR = status.errors;
                byte_counter = 1;
                break;
                
            default:
                USIDR = ERR_INVALID_CMD;
                status.errors++;
                return;
        }
    } else {
        switch(status.last_command) {
            case CMD_READ_COUNT:
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    count_value = encoder_count;
                }
                
                switch(byte_counter) {
                    case 1:
                        USIDR = (count_value >> 16) & 0xFF;
                        break;
                    case 2:
                        USIDR = (count_value >> 8) & 0xFF;
                        break;
                    case 3:
                        USIDR = count_value & 0xFF;
                        byte_counter = 0;
                        return;
                }
                break;
                
            case CMD_READ_STATUS:
                switch(byte_counter) {
                    case 1:
                        USIDR = status.overflow_count;
                        break;
                    case 2:
                        USIDR = status.last_command;
                        byte_counter = 0;
                        return;
                }
                break;
        }
        byte_counter++;
    }

    USISR = (1 << USIOIF);  // Clear overflow flag
}

// Encoder interrupt handler
ISR(PCINT0_vect) {
    // Debounce check
    uint32_t interrupt_time = micros();
    if (interrupt_time - last_interrupt_time < DEBOUNCE_TIME) {
        return;
    }
    last_interrupt_time = interrupt_time;

    // Read encoder state
    uint8_t curr_state = ((PINB & (1 << ENCODER_A)) ? 1 : 0) | 
                        ((PINB & (1 << ENCODER_B)) ? 2 : 0);
    
    uint8_t transition = (prev_state << 2) | curr_state;

    // Process encoder transition
    switch (transition) {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            if (encoder_count < MAX_COUNT) {
                encoder_count++;
            } else {
                status.overflow_count++;
            }
            break;
            
        case 0b0010:
        case 0b0100:
        case 0b1101:
        case 0b1011:
            if (encoder_count > MIN_COUNT) {
                encoder_count--;
            } else {
                status.overflow_count++;
            }
            break;
    }

    prev_state = curr_state;
}

int main(void) {
    cli();
    
    wdt_enable(WDTO_250MS);
    setup_timer();
    setup_spi();
    setup_encoder();
    
    sei();
    
    while (1) {
        wdt_reset();
        // Everything is handled by interrupts
    }

    return 0;
}

```

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

Would you like me to:
1. Add any specific features to either chip?
2. Explain the SPI communication protocol in more detail?
3. Provide example code for the master device to read these encoders?




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




#### code 
- motor control /manual or code / / slew joystick- dc motors
- track a position using an optical gates or estimation or stell 
- starmap Stellarium interface via another bitbang port x.200 commands
- long-exposure astrophotography

- read encoders, convert to angle, display on  seven segment display of 6 digits xxxx xx
  - press A gives the azimuth and alt together to 1 degree accuracy, eg az= 0299 alt = 23 displayed as 0299 23
  - press B then only az is displayed deg-min eg 0299 44 meaning 299 degs 44 minutes
  - press C then only alt is displayed deg-min eg 0023 51 meaning 23 deg 51 min
  - no need for precision in seconds unless the machanicals of the telescope can justify it  



- Stellarium integration
  - /www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/

- calibration
  - az check, true north in Sydney add 12.83°E in 2020 changes 0.04° annually
  - level
  - calibrate with known celestial object


### scope-1.mint
its all messed up have to fix it!



 

![image](https://github.com/user-attachments/assets/2b6cd303-1f15-4b7e-a85e-055055a94236)

![image](https://github.com/user-attachments/assets/cb468bee-9abd-48b9-9c95-75f166703ad7)
![image](https://github.com/user-attachments/assets/3cc8b127-2a64-4820-9200-6a2e945ad0da)


