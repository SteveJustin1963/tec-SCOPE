

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
**MINT Program to Read a Differential Encoder and Measure Altitude Between 0-90 Degrees**

---

This program reads from two input ports connected to a differential (quadrature) encoder attached to a telescope. It measures the altitude between 0 and 90 degrees by decoding the quadrature signals and converting the pulse count into degrees, minutes, and seconds.

---

**Complete MINT Program:**

```mint
// Initialize variables
1 p !                            // p = Port number for encoder signal A
2 q !                            // q = Port number for encoder signal B
[0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0 ] o !  // o = Direction array for quadrature decoding
100 e !                          // e = Pulses per degree (adjust based on encoder resolution)
e 90 * m !                       // m = Maximum pulse count (for 90 degrees)
0 c !                            // c = Pulse count initialized to 0

// Read initial encoder state
p /I s !                         // Read signal A into s
q /I t !                         // Read signal B into t
s t { | a !                      // Combine s and t into previous state a

// Start infinite loop
/U (
  // Read current encoder state
  p /I s !                       // Read signal A into s
  q /I t !                       // Read signal B into t
  s t { | b !                    // Combine s and t into current state b

  // Decode quadrature signals
  a 4 * b + n !                  // n = (a << 2) + b, transition code
  o n ? r !                      // r = Direction from direction array o[n]

  // Update pulse count based on direction
  r 0 > (
    c 1 + c !                    // If r > 0, increment pulse count c
  ) /E (
    r 0 < (
      c 1 - c !                  // If r < 0, decrement pulse count c
    )
  )

  // Ensure pulse count is within 0 and maximum count
  c 0 < (
    0 c !                        // If c < 0, set c to 0
  )
  c m > (
    m c !                        // If c > m, set c to maximum count m
  )

  // Update previous state
  a b !                          // Set previous state a to current state b

  // Convert pulse count to degrees, minutes, and seconds
  c e / d !                      // d = c / e, degrees
  c e % f !                      // f = c % e, fractional pulses after degrees
  f 60 * g !                     // g = f * 60
  g e / j !                      // j = g / e, minutes
  g e % h !                      // h = g % e, fractional pulses after minutes
  h 60 * i !                     // i = h * 60
  i e / k !                      // k = i / e, seconds

  // Output the angle
  `Degrees: ` d . /N             // Print degrees
  `Minutes: ` j . /N             // Print minutes
  `Seconds: ` k . /N             // Print seconds
)
```

---

**Explanation:**

1. **Initialization:**

   - **Port Numbers:**
     - `1 p !` assigns port `1` to variable `p`, representing encoder signal **A**.
     - `2 q !` assigns port `2` to variable `q`, representing encoder signal **B**.

   - **Direction Array:**
     - `[0 1 -1 0 -1 0 0 1 1 0 0 -1 0 -1 1 0 ] o !` initializes the direction array `o` for quadrature decoding. This array maps transitions to movement directions.

   - **Pulses per Degree and Maximum Count:**
     - `100 e !` sets pulses per degree `e` (adjust based on your encoder's resolution).
     - `e 90 * m !` calculates the maximum pulse count `m` corresponding to 90 degrees.

   - **Pulse Count Initialization:**
     - `0 c !` initializes the pulse count `c` to zero.

2. **Reading Initial Encoder State:**

   - **Read Signals:**
     - `p /I s !` reads signal **A** from port `p` into variable `s`.
     - `q /I t !` reads signal **B** from port `q` into variable `t`.

   - **Combine Signals into Previous State:**
     - `s t { | a !` combines `s` and `t` into a two-bit number `a`, representing the previous state.

3. **Main Loop:**

   - **Start Infinite Loop:**
     - `/U (` begins an infinite loop.

   - **Read Current Encoder State:**
     - `p /I s !` reads signal **A** into `s`.
     - `q /I t !` reads signal **B** into `t`.
     - `s t { | b !` combines `s` and `t` into a two-bit number `b`, representing the current state.

   - **Quadrature Decoding:**
     - `a 4 * b + n !` computes the transition code `n` by combining previous and current states.
     - `o n ? r !` retrieves the movement direction `r` from the direction array `o` using index `n`.

   - **Update Pulse Count:**
     - If `r > 0`, increment pulse count `c`.
     - If `r < 0`, decrement pulse count `c`.

     ```mint
     r 0 > (
       c 1 + c !
     ) /E (
       r 0 < (
         c 1 - c !
       )
     )
     ```

   - **Clamp Pulse Count within Limits:**
     - Ensure `c` stays between `0` and `m` (maximum pulse count for 90 degrees).

     ```mint
     c 0 < (
       0 c !
     )
     c m > (
       m c !
     )
     ```

   - **Update Previous State:**
     - `a b !` updates the previous state `a` to the current state `b`.

   - **Convert Pulse Count to Degrees, Minutes, and Seconds:**
     - **Degrees (`d`):** `c e / d !` calculates degrees by integer division of pulse count by pulses per degree.
     - **Fractional Pulses (`f`):** `c e % f !` calculates the remaining pulses after degrees.
     - **Total Minutes (`g`):** `f 60 * g !` converts fractional pulses to minutes.
     - **Minutes (`j`):** `g e / j !` calculates minutes by dividing total minutes by pulses per degree.
     - **Fractional Pulses After Minutes (`h`):** `g e % h !` calculates remaining pulses after minutes.
     - **Total Seconds (`i`):** `h 60 * i !` converts remaining pulses to seconds.
     - **Seconds (`k`):** `i e / k !` calculates seconds.

   - **Output Angle:**
     - Prints the degrees, minutes, and seconds to the console.

     ```mint
     `Degrees: ` d . /N
     `Minutes: ` j . /N
     `Seconds: ` k . /N
     ```

   - **End of Loop:**
     - The loop continues indefinitely, continuously updating the angle as the encoder signals change.

---

**Variables Used:**

- **Ports and Signals:**
  - `p`: Port number for signal **A**.
  - `q`: Port number for signal **B**.
  - `s`: Temporary variable for reading signal **A**.
  - `t`: Temporary variable for reading signal **B**.

- **Quadrature Decoding:**
  - `a`: Previous state of the encoder signals.
  - `b`: Current state of the encoder signals.
  - `n`: Transition code combining `a` and `b`.
  - `o`: Direction array for quadrature decoding.
  - `r`: Movement direction (-1, 0, or +1).

- **Pulse Count and Limits:**
  - `c`: Pulse count (number of pulses counted).
  - `e`: Pulses per degree (encoder resolution).
  - `m`: Maximum pulse count corresponding to 90 degrees.

- **Angle Calculation:**
  - `d`: Degrees.
  - `f`: Fractional pulses after degrees.
  - `g`: Total minutes (fractional pulses multiplied by 60).
  - `h`: Fractional pulses after minutes.
  - `i`: Total seconds (remaining pulses multiplied by 60).
  - `j`: Minutes.
  - `k`: Seconds.

---

**Notes and Considerations:**

- **Pulses per Degree (`e`):** Adjust `e` based on your encoder's resolution (number of pulses per degree).

- **Port Numbers (`p` and `q`):** Set `p` and `q` to the correct port numbers for your hardware configuration.

- **Quadrature Encoder Signals:** This program assumes that the encoder outputs standard quadrature signals where transitions correspond to movement.

- **Speed Limitations:** Since MINT runs on an 8-bit microprocessor and uses polling to read signals, it may not handle high-speed encoder signals effectively. This program is suitable for slow-moving applications, such as manually adjusting a telescope.

- **Continuous Output:** The program continuously outputs the angle. You may modify it to update the display only when the angle changes or at specific intervals.

- **No Debouncing:** This program does not implement signal debouncing. If your encoder produces noisy signals, consider adding debouncing logic.

---

**Understanding Quadrature Decoding in This Program:**

- **Quadrature States:** The encoder signals **A** and **B** can be combined into four states (00, 01, 10, 11).

- **Transition Code (`n`):** By shifting the previous state `a` left by 2 bits and adding the current state `b`, we get a unique transition code `n` for each state change.

  ```mint
  a 4 * b + n !    // n = (a << 2) + b
  ```

- **Direction Array (`o`):** The array `o` maps each transition code to a movement direction:

  - `+1`: Forward movement (increment pulse count).
  - `-1`: Backward movement (decrement pulse count).
  - `0`: No movement or invalid transition.

- **Updating Pulse Count:**

  - **Forward Movement:** If `r` is `+1`, increment `c`.
  - **Backward Movement:** If `r` is `-1`, decrement `c`.

---

**Example Usage:**

Assuming your encoder has 100 pulses per degree:

- **Adjust Pulses per Degree:**

  ```mint
  100 e !    // If your encoder has 200 pulses per degree, use 200 e !
  ```

- **Running the Program:**

  - Load the program into your MINT interpreter.
  - Ensure the encoder signals are connected to the correct ports.
  - Run the program. It will continuously display the altitude angle as the telescope moves.

---

**Testing the Program:**

To test the program without actual hardware:

- **Simulate Encoder Signals:**

  - Modify the signal reading part to simulate encoder input by manually changing `s` and `t`.

- **Example Simulation:**

  ```mint
  // Simulate signals
  0 s !    // s = 0 or 1 (simulate signal A)
  1 t !    // t = 0 or 1 (simulate signal B)
  ```

- **Step Through Transitions:**

  - Manually change `s` and `t` to simulate encoder transitions and observe the output.

---

**Conclusion:**

This MINT program reads from two input ports connected to a differential encoder and calculates the telescope's altitude between 0 and 90 degrees. It demonstrates quadrature decoding, pulse counting, and conversion of pulses to degrees, minutes, and seconds within the constraints of the MINT language and an 8-bit microprocessor environment.

Feel free to adjust the pulses per degree, port numbers, and any other parameters to match your specific hardware setup. If you have any questions or need further assistance, please let me know!




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



  


