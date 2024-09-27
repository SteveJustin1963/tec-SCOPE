

Integrating your tec1d with your 8-inch F5 Dobson telescope

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)



- convert push around scope add motors and encoders
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)

![image](https://github.com/user-attachments/assets/a65c79ff-79d1-4115-a1bc-1dc1dbe430ba)

- motors
  - dc motors , massage motors, cheap geared motors, ebau, ali etc



 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)

- encoders
  - HN3806-AB, AB 2 phase rectangular orthogonal pulses 600 pulses/rev, max 5000rpm, response 0-20KHz 
  - green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
  - should not connect AB directly VCC 
  - think about using LS7183



calibrate
- angle check using iphone for compass,
  - it has a level guide in it as well with the little cross in the middle. adjust thumb screw in base
  - true north in Sydney add 12.83°E in 2020 changes 0.04° annually
- a plastic protractor and plumb for angle
- once positioned right then calibrate with alignment to a known celestial object

mount
- we are using alt and azimuth, like x and y
- we need 2 motors and 2 encoders
- the dobson needs to be modified so movement is made on shafts and gears
- gears
  - rocker base atatch inverted drive belt with the teeth facing out so interfaces with Belt Drive Pulley to its 6mm shaft

![image](https://github.com/user-attachments/assets/27685599-0ccf-404d-9e11-eb6efa607bb2)

- Harmonic+Reducer
- worm drive

control
- we will ise a simple  x y joystick slew it around with the dc motors
- and also via via tec1 commands
- or track a position using an optical gates or estimation 
- will try starmap Stellarium interface via another bitbang port x.200 commands
- what to do for long-exposure astrophotography ?

 
code 

display angle
- read data from encoders, convert to angle, display on tec1s seven segment display of 6 digits xxxx xx
- press A gives the azimuth and alt together to 1 degree accuracy, eg az= 0299 alt = 23 displayed as 0299 23
- press B then only az is displayed deg-min eg 0299 44 meaning 299 degs 44 minutes
- press C then only alt is displayed deg-min eg 0023 51 meaning 23 deg 51 min
- no need for precision in seconds unless the machanicals of the telescope can justify it  

### encoder.MINT





### encoder-int.MINT

**Updated MINT Program to Handle Interrupts and Adjusted Pulses per Degree**

---

**Introduction:**

Given your requirements:

- **Interrupt Handling:** Implement interrupt handling in MINT to update the altitude only when the telescope moves.
- **Encoder Specifications:** The differential rotary encoder outputs 600 pulses per 360 degrees, geared 10:1 to increase it to 6000 pulses per 360 degrees.
- **Altitude Measurement Range:** Measure altitude between 0-90 degrees.
  
We'll update the MINT code to:

- Use interrupts to read the encoder signals only when movement occurs.
- Adjust calculations to accommodate the new encoder resolution (6000 pulses per 360 degrees, or 1500 pulses per 90 degrees).
- Allow the main program to perform other tasks when the telescope is stationary.

---

**MINT Interrupt Handling Overview:**

In MINT, interrupts are handled by defining a function stored in the uppercase variable `Z`. When an interrupt occurs, MINT executes the function `Z`. You can determine which interrupt occurred by checking the `/v` system variable.

**Key Points:**

- **Define Interrupt Handler:** `:Z ... ;`
- **Trigger Interrupts:** Hardware interrupts can be triggered via external circuitry connected to the Z80's interrupt lines.
- **Variables:**
  - `/v`: Contains the vector of the interrupt that occurred.
  - `/X`: Executes machine code at a given address.

---

**Updated MINT Code:**

```mint
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
```

---

**Explanation:**

1. **Variables Initialization:**

   - **Port Numbers:**
     - `1 p !` assigns port `1` to variable `p` for encoder signal **A**.
     - `2 q !` assigns port `2` to variable `q` for encoder signal **B**.

   - **Quadrature Direction Array:**
     - `[0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0 ] o !` initializes the direction array `o` for quadrature decoding.

   - **Pulse Count and Maximum:**
     - `1500 m !` sets the maximum pulse count `m` corresponding to 90 degrees (since 6000 pulses per 360 degrees, so 1500 pulses per 90 degrees).
     - `0 c !` initializes the pulse count `c` to zero.

   - **Degree Calculation Factors:**
     - `3 f_d !` sets the numerator for degrees calculation.
     - `50 f_div !` sets the denominator for degrees calculation.

2. **Initial Encoder State:**

   - **Reading Signals:**
     - `p /I s !` reads signal **A** into `s`.
     - `q /I t !` reads signal **B** into `t`.
   - **Combining Signals:**
     - `s t { | a !` combines `s` and `t` into a two-bit number `a`, representing the previous state.

3. **Interrupt Handler (`:Z ... ;`):**

   - **Reading Current Encoder State:**
     - `p /I s !` reads signal **A** into `s`.
     - `q /I t !` reads signal **B** into `t`.
     - `s t { | b !` combines `s` and `t` into a two-bit number `b`, representing the current state.

   - **Quadrature Decoding:**
     - `a 4 * b + n !` computes the transition code `n` by combining previous and current states.
     - `o n ? r !` retrieves the movement direction `r` from the direction array `o` using index `n`.

   - **Updating Pulse Count:**
     - If `r > 0`, increment `c`.
     - If `r < 0`, decrement `c`.
     - Ensure `c` stays within `0` and `m` (maximum pulse count for 90 degrees).

   - **Update Previous State:**
     - `a b !` updates the previous state `a` to the current state `b`.

   - **Calculating Degrees, Minutes, and Seconds:**
     - **Degrees (`deg`):**
       - `c f_d * d !` computes `d = c * f_d`.
       - `d f_div / deg !` computes `deg = (c * f_d) / f_div`.
     - **Remaining Pulses (`rem_p`):**
       - `d f_div % rem_p !` computes `rem_p = (c * f_d) % f_div`.
     - **Minutes (`min`):**
       - `rem_p 60 * m1 !` computes `m1 = rem_p * 60`.
       - `m1 f_div / min !` computes `min = (rem_p * 60) / f_div`.
     - **Remaining Pulses after Minutes (`rem_s`):**
       - `m1 f_div % rem_s !` computes `rem_s = (rem_p * 60) % f_div`.
     - **Seconds (`sec`):**
       - `rem_s 60 * s1 !` computes `s1 = rem_s * 60`.
       - `s1 f_div / sec !` computes `sec = (rem_s * 60) / f_div`.

   - **Output Angle:**
     - The interrupt handler prints the updated degrees, minutes, and seconds.

4. **Main Program:**

   - The main program enters an infinite loop (`/U (`) and performs other tasks.
   - In this example, it prints a message and includes a wait loop to simulate processing time.
   - **Note:** Adjust the wait loop (`1000 ( )`) as needed for your application.

---

**Understanding the Degree Calculation:**

- **Pulse Count to Degrees:**

  The total pulses for 90 degrees is 1500. Therefore, to calculate the degrees from the pulse count `c`, we use:

  ```mint
  Degrees = (c * 90) / 1500
  ```

  Simplifying the fraction:

  ```mint
  (c * 90) / 1500 = (c * 3 * 30) / (50 * 30) = (c * 3) / 50
  ```

  Thus, we set:

  ```mint
  f_d = 3     // Factor numerator
  f_div = 50  // Factor denominator
  ```

- **Minutes and Seconds Calculation:**

  - After computing degrees, the remaining fraction is converted to minutes and seconds by multiplying by 60 and dividing by `f_div` accordingly.

---

**Adjustments for Encoder Resolution:**

- **Encoder Pulses:** The encoder outputs 6000 pulses per 360 degrees due to the 10:1 gearing, resulting in 1500 pulses per 90 degrees.
- **Pulse Count Range:** The pulse count `c` ranges from `0` to `1500` for altitudes between 0 and 90 degrees.
- **Integer Arithmetic:** By using integer arithmetic and appropriate scaling factors (`f_d` and `f_div`), we avoid fractional calculations.

---

**Interrupt Handling Mechanism:**

- **Triggering Interrupts:**

  - The encoder circuitry should be designed to generate an interrupt signal (e.g., by connecting to the Z80's `INT` line) whenever the encoder detects movement.
  - Each pulse from the encoder triggers an interrupt, causing MINT to execute the function stored in `Z`.

- **Function `Z`:**

  - The interrupt handler `Z` updates the pulse count and recalculates the altitude angle.
  - Since the main program is not constantly polling the encoder, it can perform other tasks when the telescope is stationary.

---

**Testing the Program:**

- **Simulating Encoder Movement:**

  - To test the program without hardware, you can simulate encoder signals by manually changing the values of `s` and `t` in the interrupt handler.
  - Alternatively, you can generate software interrupts by calling the interrupt handler directly (e.g., `Z`).

- **Expected Outputs:**

  - As the pulse count `c` changes, the program will output the corresponding degrees, minutes, and seconds.
  - For example, at `c = 750` (halfway to 90 degrees):

    ```
    Degrees: 45
    Minutes: 0
    Seconds: 0
    ```

---

**Important Considerations:**

1. **Hardware Setup:**

   - Ensure that the encoder is correctly connected to the input ports (`p` and `q`).
   - The interrupt signal from the encoder must be properly connected to the Z80's `INT` or `NMI` line.

2. **Encoder Signal Debouncing:**

   - Mechanical encoders may produce noisy signals. Consider adding hardware or software debouncing if needed.

3. **Interrupt Priorities:**

   - If other interrupts are used in your system, make sure they are properly prioritized and handled to avoid conflicts.

4. **Processor Limitations:**

   - The Z80 processor and MINT interpreter may have limitations in handling very high-speed interrupts. Ensure that the interrupt rate does not exceed what the system can handle.

5. **Main Program Behavior:**

   - The main program can perform any other tasks required by your application.
   - Be cautious of any shared resources between the main program and the interrupt handler to avoid race conditions.

---

**Conclusion:**

By incorporating interrupt handling and adjusting the calculations for the new encoder resolution, the updated MINT program efficiently measures the altitude between 0 and 90 degrees when the telescope moves. The main program remains free to perform other tasks when the telescope is stationary, optimizing the use of system resources.

**Feel free to modify the code to suit your specific hardware setup and application needs. If you have any questions or need further assistance, please let me know!**




 
ideas
- https://www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/



  


