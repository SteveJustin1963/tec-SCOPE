Back in 1977 when I was 14, my Star Wars nerd brain incessantly checked adverts in the Comics and TV-Week for science things, like ray guns, crystal radios, ufo detector kits etc. That day was something new came up, "Make your own telescope !" by Astro Optical at Crows Nest Sydney. So I showed Dad and got his support. My kid brain thought it would be instant, instead it was hard work with delayed gratification, so after 1 year of polishing the glass on our home made Spiro graph machine, then lapping with tar + wood flour, then buffing, then testing then adding the reflective coating.... the 12 inch mirror was finished. Next we welded together a steel equatorial mount, added gears, knobs, a huge lead counter weight, put the optics in a fabricated metal + fiberglass tube. After much aligning it worked. We even had to chop open and modify the carport roof to open to the stars. Now 42 years on it’s still in my possession, dusty and sad but complete. Then in the early 80s came along Talking Electronics and the TEC1 Z80. I lapped up every page and made every project. Sadly I lost the original then in 2022 i got a another pcb via faceboook and ebay and tec1-d was reborn. I also acquired a cheap circa 1980s wooden truss Dobson telescope with a 8 inch mirror rated at F5 ratio. A new chance emerged to integrate telescope with a tec1 of the same era.  What can we see and do? Running the numbers, eye mag of x6 and F5 + 8 inch mirror gives 8 degrees field of view, thats around 0.7 arc seconds between two points of light; wow thats cool!
![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)

So with the Tec-1 Z80 SBC and some extra parts; motors and encoders etc, this will elevate our stargazing game by harnessing the power of the mighty Tec-1 to control the telescope. The simple friction controlled Alt-Azimuth mount will be converted to a geared mount driven my motors and read with encoders to get a leap in efficiency and precision for slewing and tracking. 

![image](https://github.com/user-attachments/assets/175f6647-cef5-44bb-8ab4-b49f28b1b4a6)


### display
The Tec-1 has 6 digits of display to show the angle of degrees and minutes 
arranged as xxxx and xx for address and data per its default mode in the monitor program.

press A then as you move the scope the numbers will update, as azimuth-alt to 1 degree 
- 0299 23 is 299 degs 23 deg
press + once then only alt is displayed to higher precision.
- 0299 44 is 299 degs 44 minutes
press - then only az is displayed to higher precision.
- 0023 16     ie 23 degs 16 minutes
press A then back to azimuth-alt to 1 degree



more detail we can drop the leading number  
az ddmm ss
az 100 to 359 we show d.d.mm ss, use the . or .. or ... to show 1, 2 or 3
example 4.544 12 = 145 deg 44 min 12 sec
example 4.5.44 12 = 245 deg 44 min 12 sec
example 4.5.4.4 12 = 345 deg 44 min 12 sec 
alt ddmm ss








## Dobsons = a simple type of reflector telescope
Dobsons are just pushed by hand on a low friction mount. This system works really well and settles very fast after pushing. but depending where you're looking, stars or objects move out of view due to the earth's rotation. you don't notice that when you look up with your eyes, but as soon as you look through the scope the moon has moved incredibly fast. Every minute you have to keep pushing to keep up, but as soon as you do we always push the wrong way as the image is inverted. we have to take our eye away and do it externally or with a laser pointer or dot finder. I have even pulled the eyepiece out (mine is a very loose fit) and watched the reflection of light in the eyepiece tube to get a sense of direction to go in. Also small pushes to keep up are always too big and we lose the target anyway. fixing this with software and hardware would be nice.


## reading
 we need to read the alt and azimuth (and later inputting). They are angle measurements. We need to locate the axis of these and mount some kind of reader and turn that into numbers we can crudely use, such as a mechanical compass and protractor, or their electronic equivalents. an angle gauge and or a compass can be had for under $20 online from Asia online. When getting the azimuth from the compass don't forget to compensate and convert magnetic north to true north, Sydney needs about 12 added (magnetic declination was 12.83°E in 2020 with 0.04° annual change). It varies all over the world. Now we have a calibration reference. next step in measuring electronically we could measure with a simple R pot and feed this into a MCU that has ADC but our tec1 lacks this in this design so we instead make R part of an 555 oscillator circuit, once calibrated for a correct starting position, R will change at various points of rotation so we interpolate the missing R values in between and then convert them into angles, so the period or freq wrt a counter or code loop can be converted to angles.


## cct
![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

- circuit simulator using 555 ic https://tinyurl.com/2p8fdmws
- make the 220 value the pot
- am aiming for freq between 100 and 1000 and no more than 3000

![image](https://user-images.githubusercontent.com/58069246/220818068-d6cbff32-57e3-4e6c-9fc0-8369b4c0e593.png)



## another 555 cct
https://www.electronics-lab.com/ne555-timer-sparks-low-cost-voltage-to-frequency-converter/

![image](https://github.com/SteveJustin1963/tec-SCOPE/assets/58069246/5af4808d-1427-46c5-b78d-ee9c48157a15)


## 74HC590 8-bit binary counter cct
- https://www.google.com/search?q=74HC590&rlz=1C1FKPE_en-GBAU984AU984&sourceid=chrome&ie=UTF-8
- Encoder counter chip https://www.usdigital.com/products/accessories/interfaces/ics/

## cap cct
 - https://www.instructables.com/Simple-Capaitance-Meter/



## let's dream about the code possibilities
 What do we want it to do? start with
 - Defines variables for storing angle, frequency, and motor direction.
- Initialises hardware components and communication interfaces.
- Includes routines for reading angle and calculating frequency.
- Converts R to frequency.
- Controls motors and slewing, including manual and automatic slewing.
- Reads optical encoders and integrates rotary encoders.
- Implements auto-tracking, guiding, and go-to functionality.
- Provides alignment assistance with feedback during the process.
- Optimises performance for resource-constrained platforms.
- Sets up the main program loop to execute functions repeatedly.
- Handles telescope control system communication and external communication with Stellarium.
- Manages camera control and imaging.
- Includes error handling and exception handling.
- Allows for additional features and functionalities to be added.
- Handles user input and commands for manual control and configuration.


 ```pseudo
\ Include necessary headers and libraries here

\ Define constants and variables
VARIABLE angle \ Store the telescope's current angle
VARIABLE frequency \ Store the calculated frequency
VARIABLE motor_direction \ Store the motor direction (0 for stop, 1 for forward, -1 for backward)

\ Initialize hardware components and communication interfaces
: initialize-telescope
 \ Initialize electronic level, compass, ADC, and motor control
 \ Set up communication with Stellarium
 \ Configure rotary encoder
 \ Initialise optical encoders for alt and az axes

\ Read angle and calculate frequency
 : read-angle-and-frequency
 \ Code for reading angle using the electronic level and compass
 \ Code for generating pseudo-period with counter loop from the R value from pot angle using 555 oscillator circuit
 \ Calibration and interpolation

\ Voltage-to-Frequency Conversion
: voltage-to-frequency
 \ Code for R-to-frequency conversion using NE555
 \ Code for 8254 interval timer
 \ Calibration and interpolation

\ Motor Control and Slewing
: motor-control-and-slewing
 \ Code for controlling DC motors using worm drives
 \ Implement non-linear interpolation and trigonometry for precise angle control
 \ Handle manual slewing using switches and joystick
 \ Implement automatic slewing for go-to functionality
 \ Enable remote control via TCS
 \ Track telescope position using optical gate

\ Read Differential Optical Encoders
: read-optical-encoders
 \ Read optical encoders for alt and az axes
 \ Calculate precise position information

\ Rotary Encoder Integration
: integrate-rotary-encoder
 \ Code for reading the rotary encoder HN3806-AB
 \ Interface with Timing Belt Drive Pulley
 \ Ensure precise angle measurement and control

\ Auto-Tracking and Guiding
: auto-tracking-and-guiding
 \ Implement automatic tracking to compensate for Earth's rotation
 \ Integrate guiding capabilities to maintain precise object tracking
 \ Support long-exposure astro-photography

\ Go-To Functionality
: go-to-functionality
 \ Implement go-to capabilities to automatically slew to specific objects
 \ Calculate necessary coordinates for precise pointing
 \ Provide user input or database selection for target objects

\ Alignment Assistance
: alignment-assistance
 \ Implement alignment assistance routines to aid users in aligning the telescope accurately with celestial objects
 \ Provide feedback or guidance during the alignment process to improve user experience

\ Performance Optimisation
: performance-optimisation
 \ Optimize the software's performance to minimize resource usage and maximize efficiency, particularly on resource-constrained platforms like the Z80 SBC
 \ Profile the code to identify bottlenecks and optimize critical
sections for improved responsiveness

\ Main Program Loop
: main-loop
 initialize-telescope
 BEGIN
 read-angle-and-frequency
 R-to-frequency
 motor-control-and-slewing
 read-optical-encoders
 integrate-rotary-encoder
 auto-tracking-and-guiding
 go-to-functionality
 alignment-assistance
 performance-optimisation
 \ Additional functionalities (camera control, photo compositing, etc.)
 AGAIN

\ Telescope Control System (TCS)
: tcs-control
 \ Define communication protocols for remote control
 \ Handle commands from remote devices

\ Camera Control and Imaging
: camera-control
 \ Implement camera control and image capture
 \ Explore Lucky Imaging techniques

\ External Communication
: stellarium-communication
 \ Establish communication with Stellarium
 \ Send telescope position
 data
 \ Receive and execute movement commands from Stellarium

\ Compile and run the main program
main-loop

\ Handle user input and commands
: handle-user-input
 \ Implement user interface for manual control and configuration

\ Include error handling and exception handling routines as needed

\ Additional features and functionalities can be added as separate code blocks

\ End of the software set
```




### 1.0
- configure a timer/counter
- read its value,
- calculate the freq, and store the result in memory.
- The div16 subroutine is used to perform a 16-bit division operation by shifting bits,

 ### 1.01
- try 8254 is for timing and counting
- has three 16-bit counters (Counters 0, 1, and 2) with various operating modes.
Key elements:
- Registers: timer_value, frequency_low, frequency_high.
- Ports: pit_control_port, pit_data_port.
- Constants: frequency_divisor, clock_frequency.

Program execution:
- Initializes Counter 0 in square wave mode.
- Reads counter value, calculates frequency using div16 subroutine.
- Stores frequency in frequency_low and frequency_high.
- div16 subroutine divides a 16-bit value by 16 for frequency calculation.


### 2.0
- reads values from a specific port,
- averages them,
- calculates an angle based on the average value,
- performs some calibration and conversion routines,
- and uses lookup tables for division and multiplication to optimize calculations

### 3.0
- measure the frequency of a signal on bit 1 of Port 06 within the range of 200 to 1000 Hz
 - code assumes that the incrementing of the HL register represents a known time interval, which can be used to calculate the frequency.
- For more accurate and efficient frequency measurement, it is recommended to use dedicated timer hardware

### mp-1.f
- main program in forth
- todo convert to fixed point arithmetic with groups of integers

Storing precision in groups of integers, often referred to as fixed-point arithmetic, can be a way to maintain better accuracy than using plain 16-bit integers for certain calculations, especially when dealing with fractional values. Fixed-point arithmetic allows you to represent fractional numbers with a fixed number of integer bits and fractional bits. This approach can help you maintain precision while using integer-based data types.

Here's how you might apply fixed-point arithmetic to the provided code:

1. **Choose a Fixed-Point Format**: Determine how many bits you want to allocate for the integer part and how many bits for the fractional part of your fixed-point numbers. This allocation depends on the required precision for your calculations. For example, you might use a Q16.16 format, which uses 16 bits for the integer part and 16 bits for the fractional part.

2. **Scale Values**: Scale your input values (e.g., angles) and constants accordingly. When you read input values like degrees, convert them to your fixed-point format by multiplying them by a suitable scaling factor (e.g., 2^16 for Q16.16).

3. **Perform Calculations**: Perform calculations using fixed-point arithmetic. When multiplying or dividing fixed-point numbers, consider the scaling factor. For example, when multiplying two Q16.16 fixed-point numbers, you would scale the result down by shifting right by 16 bits.

4. **Convert Back**: When you need to display or use the results, convert them back to their original units by reversing the scaling factor.

Here's an example of how you might modify the code to use Q16.16 fixed-point arithmetic:
-mp16.16.f

In this modified code, the `toFixedPoint` function scales input values to the Q16.16 format, and the `fromFixedPoint` function converts results back to degrees when displaying them. The `hourangle`, `declination`, `lst`, and `rightascension` variables are declared with 32 bits to accommodate the fixed-point format.

Using fixed-point arithmetic in this manner allows you to maintain better precision than using plain 16-bit integers for calculations involving fractional values. However, it requires careful handling of scaling factors and conversions between fixed-point and real-world units.

#### Q16.16 is a fixed-point representation format
 where numbers are divided into two parts: a 16-bit integer part and a 16-bit fractional part. It's commonly used in embedded systems and programming when you need to work with fractional numbers but don't want to use floating-point arithmetic due to its computational overhead or limited hardware support.

Here's how Q16.16 fixed-point format works:

- The first 16 bits represent the integer part of the number.
- The next 16 bits represent the fractional part of the number.

To convert a Q16.16 fixed-point number to a real-world value, you divide the 32-bit fixed-point number by 2^16 (65536). Conversely, to convert a real-world value to the Q16.16 format, you multiply it by 2^16.

Here's a simple example to illustrate the concept:

1. Real-world value: 3.5
 - Convert to Q16.16: 3.5 * 65536 = 229376 (which is 0x00038000 in hexadecimal).

2. Q16.16 value: 0x00038000
 - Convert to real-world: 0x00038000 / 65536 = 3.5

In your code, using Q16.16 fixed-point format allows you to perform arithmetic operations on fractional values with integer operations, which can be more efficient on certain hardware platforms and avoids the complexities of floating-point arithmetic.

The loss in accuracy between Q16.16 fixed-point and 32-bit floating-point representations primarily depends on the range of values you need to represent and the precision required for your specific application. Here's a general comparison of the two formats:

1. **Q16.16 Fixed-Point Format**:
 - **Precision**: The Q16.16 format provides 16 bits of fractional precision, which means it can represent values with a fractional part as small as 1/65536.
 - **Range**: It can represent values within the range of approximately -32768 to 32767 with fractional values in the range of -1 to (1 - 1/65536).

2. **32-bit Floating-Point Format** (IEEE 754 Single Precision):
 - **Precision**: Single-precision floating-point numbers provide about 24 bits of precision in the significant and (fractional part), which allows for much finer granularity in representing fractional values compared to Q16.16.
 - **Range**: They have a much larger range of representable values, typically from approximately -3.4e38 to 3.4e38 (positive and negative), which is a significantly wider range compared to Q16.16.

**Loss of Precision**:
- When using Q16.16 fixed-point, you may experience a noticeable loss of precision when working with small fractional values or when performing multiple consecutive operations, as the fractional part can only represent values down to 1/65536.
- In contrast, 32-bit floating-point numbers can represent much smaller fractions and a wider range of values without significant loss of precision.

**Example**:
Consider representing the value 0.0001:
- In Q16.16, it would be approximately 6.1035e-06 (which is 6.1035 / 65536).
- In 32-bit floating-point, it can be precisely represented as 1.0e-4.

If you need to perform calculations that require high precision, especially with very small fractional values or a wide range of values, 32-bit floating-point would generally be a better choice. However, if memory or computational efficiency is a concern, Q16.16 fixed-point can still be suitable for many applications with reasonable precision requirements.

In summary, the loss in accuracy between Q16.16 fixed-point and 32-bit floating-point depends on the specific numerical range and precision requirements of your application. For tasks involving very small or large values and extensive calculations, 32-bit floating-point offers superior precision, while Q16.16 fixed-point is more efficient for tasks within its narrower range and precision capabilities.

Q16.16 fixed-point format alone may not provide the accuracy of degrees, minutes, and seconds to two decimal places, especially if you want precise representations of angular values. Each decimal place in degrees corresponds to a factor of 60 (e.g., degrees to minutes or minutes to seconds), which requires more precision than Q16.16 can offer.

To achieve accuracy to two decimal places for degrees, minutes, and seconds, you would typically need a more granular representation, such as a fixed-point format with a higher number of fractional bits or, more commonly, floating-point numbers. Here's a brief overview of the requirements for achieving two decimal places of accuracy for degrees, minutes, and seconds:

1. **Degrees (0.01 degree accuracy):**
 - You can represent degrees with two decimal places by using a fixed-point format with at least 2 decimal places (e.g., Q16.16 or Q32.32) or by using a floating-point number with single precision (e.g., IEEE 754 single-precision).

2. **Minutes (0.01 minute accuracy):**
 - To represent minutes with two decimal places (0.01 minutes accuracy), you'll need a format with at least 4 fractional bits.

3. **Seconds (0.01 second accuracy):**
 - For two decimal places of seconds (0.01
seconds accuracy), you'll require a representation with at least 6 fractional bits.

If you need to work with angular values that require this level of accuracy, consider using floating-point representations, such as 32-bit or 64-bit floating-point numbers (single or double precision). Floating-point numbers are designed to handle a wide range of values with high precision and are commonly used in applications involving angles and coordinates that require sub-degree accuracy.

In summary, while Q16.16 fixed-point format can be suitable for some applications, it may not provide the required accuracy of two decimal places for degrees, minutes, and seconds. Using floating-point representations with sufficient precision is a more practical approach for achieving the desired accuracy in angular measurements.


### count

 - down 00-FF
- up, large counter 0000-FFFF



 ## 1.3 ADC
- https://github.com/SteveJustin1963/tec-ADC-DAC





# Stage 2
## pots with cam angle motors
- Two DC motor drives attached to the telescope mount using threaded rod with nuts = worm drives acting as a swinging cam
- The worm drive and cam radius design will enhance torque control and minimize backlash.
- add 2 switch joystick to enable manual slewing
 - tec1 also outputs 2 motor on off controls
- optical gate counts rotations from slotted wheel on shaft to workout movement
- calibrate at 0 and 90 degrees, also resistor pots are left on for comparison/ extra calibration
 - motor turns vs angle is non-linear due to the cam action. non-linear interpolation and trigonometry used to calculate angles
- Stellarium gets position data via serial and also can issue movement commands back to motors


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






# Stage 3
 ## remove pots and cam motors, replace with geared motor drive and rotary encoders
- remove cams, motors and resistor pots
 - replace with reduction geared DC or stepper motors and with rotary encoder
- keep manual slewing and software motor control


 ![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)




 ## Rotary Encoder HN3806-AB

 ![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2 phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2 phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

I need to add a Timing Belt Drive Pulley to its 6mm shaft. will construct a wood wheel and invert a drive belt with the teeth facing out so the drive pulley can interface with it

![image](https://user-images.githubusercontent.com/58069246/223073009-7ab32520-386b-4db5-81f6-1c0dc1765fc9.png)


## code examples

https://electropeak.com/learn/how-to-interface-a-rotary-encoder-with-arduino/


 This is an Arduino sketch for reading a rotary encoder with AB 2-phase rectangular orthogonal pulse output. The rotary encoder has two output channels, A and B, which produce pulses that are 90 degrees out of phase with each other. By reading the relative phase of the A and B pulses, the sketch is able to determine the direction of rotation of the encoder.

In the setup function, the sketch sets up the serial communication at a baud rate of 9600 and configures the pin 2 and 3 as inputs with pull-up resistors. It also sets up external interrupts on pin 0 and 1, and assigns the A and B functions to be called when the interrupts are triggered.

In the loop function, the sketch checks if the value of the counter has changed since the last time it was sent over serial. If the counter has changed, it sends the new value over serial.

The A and B functions are called whenever an interrupt is triggered on pin 0 and 1, respectively. These functions check the state of the other input pin to determine the direction of rotation of the encoder. If the other input is low, the counter is incremented (in the case of A) or decremented (in the case of B). If the other input is high, the counter is decremented (in the case of A) or incremented (in the case of B).

## read the differential and count up or down
code uses the @ operator to read the value of pins 2 and 3 before deciding whether to increment or decrement the counter variable. The @ operator retrieves the value of a memory location (in this case, the value of pins 2 and 3) and pushes it onto the data stack. The IF statement then checks the value of the top item on the data stack (either 0 or 1, depending on the value of the pin) and executes the appropriate branch.

The output of this modified code will be a series of numbers, each representing the current value of the counter variable. The value of the counter will be incremented or decremented by 1 each time an interrupt is triggered on port 2 or 3, depending on the differential signal read from those pins.


 ## main program
### mp-1.f




## Iterate
- ASR012 https://www.nve.com/Downloads/ASR012.pdf mag angle sensor
- AS5600
- from a map location slew scope to that position, issues x.200 commands that tec1 turns into x an y and then into motor control
- track a position in motion. compensate for earth’s rotation. Satellite tracking different problem
- joystick slew scope to new positions
 - remote control over modem, using defined protocols, TCS - Telescope Control System
- camera mount + camera
- photo compositing on pc, Lucky Imaging technique -
 - MINT code with and without APU help
- 7 Congruent Circles In A Regular Hexagon, use microwave oven glass.

![image](https://github.com/SteveJustin1963/tec-SCOPE/assets/58069246/17a88d9c-9f73-4d14-a2d2-7cddd23e7216)

-

 ## References
- https://www.google.com.au/sky/
- https://edu.kde.org/kstars/
- https://www.indilib.org/
- https://www.tiesncuffs.com.au/pages/custom-embroidered-patches?gclid=Cj0KCQjw-daUBhCIARIsALbkjSZYO9V_ij_ypJlVEb1Inib6CAAIR20-39V1JrN4Nm-13fe0FwE5aqgaAheQEALw_wcB
- https://github.com/SteveJustin1963/tec-Stepper-Motor-Control
- https://www.facebook.com/rDUINOScope/
- https://github.com/PeterForth/Planetarium
 - https://joecreager.com/control-an-h-bridge-with-forth/
- https://www.facebook.com/groups/623556744820045
- https://www.facebook.com/groups/2225595150855239
- https://www.facebook.com/Kiskunf%C3%A9legyh%C3%A1zi-Csillag%C3%A1szati-Egyes%C3%BClet-124849427661821/
- http://stellarium.org/
- https://www.hackster.io/neil-pollard/sky-finder-a-lego-alexa-gadget-to-discover-the-night-sky-879e46
- https://www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/
- https://www.facebook.com/profile.php?id=150875201997704&ref=br_rs
- https://create.arduino.cc/projecthub/dEskoG/rduinoscope-4d399c
- https://github.com/ForthWin/Forth2020UsersGroup/blob/master/ASTROLAB.F?fbclid=IwAR1WDOtVrYgl-IU7aDVg-y2659sf4HU1BfC0qWDlf0LNyo6Y_k4kRzJZ4yA
- https://makezine.com/projects/build-a-backyard-dobsonian-telescope/
- https://www.celestron.com/pages/celestron-pwi-telescope-control-software
- https://www.astronomywa.net.au/different-types-of-telescope-mounts.html
- https://onstep.groups.io/g/main/wiki/4414
- https://www.ngc7000.com/telescope-motorization.htm
- https://www.ngc7000.com/docu-skypikit/EN/EN_200226_DobsonPushtoBoard.pdf
- https://asa.astronomy.org.au/
- https://skywatcheraustralia.com.au/product/16-collapsible-dobsonian/
- https://en.wikipedia.org/wiki/Lucky_imaging
- https://docs.arduino.cc/built-in-examples/basics/AnalogReadSerial







