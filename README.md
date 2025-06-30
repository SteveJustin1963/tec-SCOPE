under development, not ready


 what is https://github.com/hjd1964/OnStep  ?



# Start
- read https://www.cloudynights.com/ and any online stuff on astronomy.
- get or make a cheap 8-inch F5 Dobson stle, truss or tube, pushed by handtype. I found an old truss type on gumtree for $35, but was missing an eyepiece but got an extra mirror..
- any telescope will do so long as u can mod it. 

# Aiming
- use your eyes, line up two points on the side
- use a red dot aiming device, your neck needs bending,
- best choice.. save your neck use a [9x50 Angled Finder Scope Multi-Film with Upright and Non-Reversed Image](https://www.testar.com.au/products/sky-watcher-9-x-50-right-angle-finderscope?variant=43670176432353&utm_source=google&utm_medium=organic&utm_campaign=Google%20Shopping%20feed%20app%20Australia&utm_content=SKY-WATCHER%209%20x%2050%20RIGHT%20ANGLE%20FINDERSCOPE&utm_campaign=gs-2019-12-27&utm_source=google&utm_medium=smart_campaign&gad_source=1&gclid=CjwKCAiAzPy8BhBoEiwAbnM9OzjLTUabQD3bjbE1TkDAqTKBWwvbebhQD3lGjDbNC68sNm59WremxxoClnUQAvD_BwE)
- the 6x30mm is a **waste of money** due to too narrow a view [pros and cons](https://github.com/SteveJustin1963/tec-SCOPE/wiki#red-dot-finder)
- laser pointer >1mW for star aiming, but you need an astronomy permit, https://www.police.nsw.gov.au/__data/assets/pdf_file/0008/139895/List_of_Astronomical_Societies.pdf
 

# Control your telescope
- tec1 or scc using code slew, track etc
- you need to mod the mount, gear it down, say 10-1 with motors add 2 rotary encoders for position sensing 
- send position data via 4800 serial to Stellarium to project onto star map 
- plenty more...

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)

 

# Mod the Mount
- alt (up down) and azimuth (side to side) lets say x and y, are friction coupled, convert this to gears and bearing support 
- we need to add 2 motors and 2 encoders
- for an alt motor with a reduction worm drive with perpendicular shafts on each side can be got from a discarded foot massage machine, it has a 24v motor, need to be run at a lower voltage and pwm to slow it down as its powerful and fast, fast is not what we want. 
- for the az motor need something else as we have no room under the mount
- 

 
![image](https://github.com/user-attachments/assets/f66e2597-9af3-4984-bb4c-550e19ec3681)




# Rotary encoder
- HN3806-AB (around $15), has AB 2 phase rectangular orthogonal pulses 600 pulses/rev aka 2 wires, max 5000rpm we are not going that fast, response 0-20KHz 
- green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
- do not connect AB directly VCC!! go to the chip **Use pull resistors** must limit current
- Open collector output: The output is "open collector" meaning you need to add a pull-up resistor on the microcontroller side to read the signal properly.

- A and B from the rotary encoder are part of a quadrature output.
- means the encoder uses the two signals (A and B) that are 90 degrees out of phase to determine position and direction of movement. 
- we need to counting the pulses, signals alternate between HIGH (1) and LOW (0).
- count the transitions (rising or falling edges) on each channel, we can determine the movement's distance or angle.
- If A leads B eg clockwise
- If B leads A eg counterclockwise

 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)


# Dual Geared Encoder Position System

[DGEPS1.mint](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/DGEPS1.mint)

1. Quadrature decoding for two encoders
2. Direction detection using state lookup table
3. Count accumulation
4. Angle calculation and display
5. ESC key monitoring for exit
 

# Geared 10:1 Quadrature Encoder Test with Bi-directional Motion Simulation

[QETBMS1.mint](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/QETBMS1.mint)

1. Simulates a geared encoder (10:1 ratio)
2. Shows quadrature decoding
3. Tests bi-directional motion
4. Provides angle readout
5. Demonstrates state transitions



# Counter chip LS7366R
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

[LS7366R_9511_ SPI.mint](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/LS7366R_9511_%20SPI.mint)
	

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



 

# ATtiny84/85 Quadrature Encoder Implementation
- lets try the ATtiny85 with Quadrature Encoder Implementation for $1, or the 84 if we need more gpios
- am already sick of the LS7366R due to the complexity and the high cost and hard to find it, lets do it with two attiny85 or one attiny84.
- we can just modify the exiting code and  setup using two ATtiny85s, where each chip handles one encoder and communicates as SPI slave.

[ATtiny84-QEI.ino](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/ATtiny84-QEI.ino)
[ATtiny85-QEI.ino](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/ATtiny85-QEI.ino)

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

# KMA215 for az reading 
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
  - we could do some pre processing on the attiny85 or do it from mint code

[KMA215_SPI.sudo](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/KMA215_SPI.sudo)




 


# SPI
see http://easyeda.com for schematics, links coming 

![image](https://github.com/user-attachments/assets/c596d7a4-f89f-4522-9ba0-565d6488245d)


# Generic_spi
[generic_SPI.z80](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/generic_SPI.z80)
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

# test this SPI code with a CRO
using a **(Cathode Ray Oscilloscope)**, you will need to probe the SPI signals while the program is running. Here’s how to go about it:

---

### **1. Identify SPI Signal Pins**
Since SPI consists of **four** main signals, you need to probe the following:
- **CS (Chip Select)**
- **SCK (Clock)**
- **MOSI (Master Out Slave In)**
- **MISO (Master In Slave Out) – Optional** (Your code does not read MISO)

---

### **2. Connect CRO Probes**
Attach the CRO probes to the corresponding lines at the output of the Z80 SBC:
- **CH1** → **SCK (Clock)**
- **CH2** → **MOSI (Data Out)**
- **CH3** (if available) → **CS (Chip Select)**
- **GND** → **System Ground**

---

### **3. What to Look for on the CRO**
#### **A. Chip Select (CS) Behavior**
- Should be **HIGH (1)** when idle.
- Should go **LOW (0)** at the beginning of an SPI transaction.
- Should return **HIGH (1)** after the transmission completes.
- Use **single-shot mode** on the CRO to capture the CS pulse.

#### **B. Clock Signal (SCK)**
- Should toggle between **LOW (0) and HIGH (1)** during SPI communication.
- The number of clock pulses should match the **bits sent (8 per byte).**
- Check the frequency and duty cycle to confirm timing consistency.

#### **C. MOSI (Master Data Out)**
- Should shift out **one bit per clock pulse.**
- Should align with the rising or falling edge of **SCK**, depending on the SPI mode.
- Check if the data sent matches the expected bit pattern for given inputs.

#### **D. SPI Bit Timing**
Use **Cursor Measurements** to check:
- **Clock Period (Tclk) = 1 / Frequency**
- **Bit duration (Tb) = One full clock cycle**
- **CS timing** in relation to **SCK** and **MOSI**

---

### **4. Verifying Data Transmission**
To verify that the data is correctly sent:
- Send a known **byte pattern** (e.g., `0xAA = 10101010b` or `0x55 = 01010101b`).
- Observe if the MOSI waveform matches the expected bit sequence.

---

### **5. Expected Waveforms**
- **CS should start HIGH, drop LOW during transfer, then go HIGH again.**
- **Clock (SCK) should be a clean square wave, stable and consistent.**
- **MOSI should change state in sync with SCK edges** (depending on SPI mode).
- **If using an SPI device**, you may also probe **MISO** to check the response.

---

### **6. Debugging with CRO**
| Issue  | Possible Cause |
|--------|---------------|
| **CS remains high** | SPI is not enabled or SPI routine is not executing |
| **SCK is missing** | Code not reaching SPI write function or clock line issue |
| **MOSI not changing** | Register values incorrect or data not rotating properly |
| **Glitches in signals** | Poor grounding, noise interference, or incorrect timing |

---

##  **generating a test sequence** to validate the output with a known pattern

Here’s a simple **test sequence** to verify SPI communication using your **Z80 SBC** and a **CRO**.

---

### **Test Procedure**
1. **Modify the Code** to send a known data pattern.
2. **Probe SPI lines** (CS, SCK, MOSI) on your **CRO**.
3. **Compare the expected waveforms** with actual signals.

---

### **Test Code: Send a Known SPI Pattern**
Modify your code to send a fixed sequence of bytes like `0xAA` (10101010b) and `0x55` (01010101b). This will create an alternating pattern on the MOSI line, making it easy to recognize on the CRO.

```assembly
    ld d, 0AAh   ; Register/command byte (Pattern: 10101010)
    ld e, 055h   ; Data byte (Pattern: 01010101)
    call spi_write
    halt
```

- **CS (Chip Select)** should drop LOW at the start and go HIGH after transmission.
- **SCK (Clock)** should have 8 pulses per byte.
- **MOSI (Data Out)** should output `10101010` for `0xAA`, followed by `01010101` for `0x55`.

---

### **What to Expect on the CRO**
#### **1. CS (Chip Select) Behavior**
- Should **start HIGH**, drop LOW during transmission, then return HIGH.

#### **2. SCK (Clock)**
- Should show **8 clean pulses per byte**.
- Frequency should match SPI timing.

#### **3. MOSI (Data Out)**
- **For `0xAA` (10101010)**, the MOSI line should alternate HIGH and LOW.
- **For `0x55` (01010101)**, the MOSI line should alternate in reverse.

---

### **How to Analyze the CRO Output**
1. **Use Single-Shot Mode** on the CRO to capture the SPI waveform.
2. **Set Cursor Measurements**:
   - Measure **CS pulse width** to confirm SPI transaction timing.
   - Measure **SCK frequency** to verify clock timing.
   - Compare **MOSI data** with expected bit pattern.

---

### **Debugging Checklist**
| Issue | Cause | Fix |
|-------|-------|-----|
| **No CS pulse** | SPI function not executing | Check code execution flow |
| **No SCK pulses** | Clock not being toggled | Check SPI write loop |
| **Incorrect MOSI pattern** | Data shifting incorrectly | Verify `rlc` and `adc a, a` logic |
| **Glitches or noise** | Poor grounding | Ensure short, solid ground connections |

---






# Compact_SPI
[compact_SPI.z80](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/compact_SPI.z80)
 
# To test this SPI bit-banging implementation with a **CRO (Cathode Ray Oscilloscope)**, you need to probe the correct signals and verify the expected SPI behavior.

---

### **1. Identify the SPI Signals**
The code controls SPI using the I/O port (`IOPORT = 42h`). The relevant signals are:
- **CS (Chip Select)** → Bit 2 (0F8h = `1111 1000`, 0FCh = `1111 1100`)
- **CLK (Clock Signal)** → Bit 1 (`SET 1, a` and `RES 1, a`)
- **MOSI (Master Out Slave In)** → Bit 0 (`ADC a, a` sets MOSI based on Carry Flag)
- **MISO (Master In Slave Out)** → Not used in this code (for SPI read)

### **2. Connect the CRO Probes**
- **CS Probe** → Connect to the **CS pin** of the SPI device.
- **CLK Probe** → Connect to the **Clock (CLK) pin**.
- **MOSI Probe** → Connect to the **MOSI pin**.
- **GND Probe** → Connect to **circuit ground**.

### **3. Expected Waveform Observations**
| **Signal**  | **Expected Behavior on CRO** |
|------------|----------------------------|
| **CS**    | Goes **low** (`0`) at the start of transmission and **high** (`1`) at the end. |
| **CLK**   | Generates **8 pulses** per byte during transmission (should be a square wave). |
| **MOSI**  | Transmits **1 bit per clock pulse**, shifting left from `D` and `E` registers. |

### **4. Step-by-Step Testing**
1. **Test `INIT_SPI` Routine**
   - Run `INIT_SPI` alone.
   - CRO should show **CS = HIGH**, **CLK = LOW**, and **MOSI = LOW**.

2. **Test `SPI_WRITE` Routine**
   - Load a test value into `D` (command) and `E` (data).
   - Call `SPI_WRITE`.
   - Observe:
     - **CS goes LOW** at the start.
     - **CLK produces 8 pulses** per byte.
     - **MOSI changes per bit being shifted out**.
     - **CS goes HIGH** at the end.

3. **Verify Clock Timing**
   - Use the **time/div** function on the CRO to check if the clock pulses are equally spaced.

4. **Check Data Transfer**
   - Use the **"Single Shot" mode** on the CRO to capture the **exact** MOSI waveform and compare it to the expected bit sequence.

---

### **5. Debugging with CRO**
| **Issue**  | **Possible Cause** | **Fix** |
|------------|--------------------|---------|
| No signals at all | `IOPORT` not outputting | Check Z80 I/O configuration |
| CS never goes LOW | Wrong `OUT (IOPORT), A` operation | Verify `LD A, 0F8h` before output |
| No CLK pulses | `SET 1, a` and `RES 1, a` missing or incorrect | Check loop timing |
| Wrong data on MOSI | `RLC d` or `RLC e` not shifting correctly | Verify `D` and `E` values before sending |

# test **program example** to load values into `D` and `E` before calling `SPI_WRITE`
Here’s a **test program** to verify your SPI bit-banging implementation on a Z80 system. It will send a test command (`D = 0xA5`) and data (`E = 0x3C`) via SPI and allow you to observe the signals using a CRO.

---

### **Z80 Assembly Test Program**
```assembly
IOPORT:   EQU   42h   ; Define the I/O port address

            ; Initialize SPI bus
INIT_SPI:      
            LD   a, 0fch   ; CS high, CLK low, MOSI low
            OUT  (IOPORT), a   
            RET      
                        
            ; SPI Write: D = Command/Register, E = Data
SPI_WRITE:      
            PUSH   bc   
                        
            ; Send 8 bits from D (command/register)
            LD   b, 8   
REG_LOOP:      
            LD   a, 0f8h   ; Lower CS
            RLC  d        ; Rotate left to bring bit into CF
            ADC  a, a      ; Set MOSI based on CF
            OUT  (IOPORT), a   
            SET  1, a      ; CLK high
            OUT  (IOPORT), a   
            RES  1, a      ; CLK low
            OUT  (IOPORT), a   
            DJNZ REG_LOOP  ; Repeat for 8 bits
                        
            ; Send 8 bits from E (data)
            LD   b, 8   
DATA_LOOP:      
            LD   a, 0f8h   
            RLC  e        ; Rotate left to bring bit into CF
            ADC  a, a      ; Set MOSI based on CF
            OUT  (IOPORT), a   
            SET  1, a      ; CLK high
            OUT  (IOPORT), a   
            RES  1, a      ; CLK low
            OUT  (IOPORT), a   
            DJNZ DATA_LOOP   
                        
            LD   a, 0fch   ; Raise CS to end communication
            OUT  (IOPORT), a   
            POP  bc   
            RET 

            ; MAIN TEST ROUTINE
START:
            CALL INIT_SPI    ; Initialize SPI
            LD   D, 0A5h     ; Load test command (10100101)
            LD   E, 03Ch     ; Load test data (00111100)
            CALL SPI_WRITE   ; Transmit via SPI
            HALT            ; Stop execution (for testing)
```

---

### **How to Test with a CRO**
1. **Load and run the program** on your Z80 system.
2. **Connect CRO probes** to the SPI lines:
   - **CS** → Should go **LOW** at start and **HIGH** after transmission.
   - **CLK** → Should generate **8 pulses per byte**.
   - **MOSI** → Should transmit **`D = 0xA5` (10100101)** followed by **`E = 0x3C` (00111100)**.
3. **Observe waveforms**:
   - Verify **CS timing** (active during transfer).
   - Ensure **8 clock pulses per byte**.
   - Compare **MOSI data** to the expected bit pattern.

---

### **Expected CRO Readings**
#### **Command (D = `0xA5` → `10100101`)**
```
MOSI  → 1  0  1  0  0  1  0  1
CLK   → _|-|_|-|_|-|_|-|_|-|_|-|_|-|_|-|
CS    → ______----------------______   (low during transfer)
```

#### **Data (E = `0x3C` → `00111100`)**
```
MOSI  → 0  0  1  1  1  1  0  0
CLK   → _|-|_|-|_|-|_|-|_|-|_|-|_|-|_|-|
CS    → ______----------------______   (low during transfer)
```

 



# main code loop ideas
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

 

 # Main_loop ideas test1

![image](https://github.com/user-attachments/assets/48cf9d0e-ecba-4236-a271-55e19285d0e3)

[main_loop1.mint](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/main_loop1.mint)

```
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


# 27.2.2025
working on DGEPS1.md


