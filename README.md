
# A TEC-1 Controlled Telescope

https://github.com/SteveJustin1963/tec-SCOPE/blob/master/activity_log.md

![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)

- develop a system to read and control a cheap Dobson 200mm F5 telescope 
- in three stages.


## stage -0.9 plumb bob + protractor and compass
- attached plumb bob and protractor to side and compass to base to get manual readings 



##  stage-1 - only read pot values to get angle
- 555 circuit to convert the potentiometer value = position into variable freq square pulses. 
- send to I/O port and calculate average = freq number
- calibrate; correlate at 0 and 90 degrees to freq at these positons- save
- Interpolate the in between angles from in between freq and send to 7seg display
- converted angle to star position, send to Stellarium over serial port
- Stellarium finds location on star map, compare to visual data 

## generate a variable freq from a pot
![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

circuit simulator https://tinyurl.com/2p8fdmws

make the 220 value the pot

![image](https://user-images.githubusercontent.com/58069246/220818068-d6cbff32-57e3-4e6c-9fc0-8369b4c0e593.png)

## iterate 555 freq method
- 8254
- [adc](https://github.com/SteveJustin1963/tec-ADC-DAC)


## 8254 interval timer
if we can guess a duration from the code internally while we read the freq pulses we maybe able to gauge the angle. but if not then use an 
8254 Programmable Interval Timer (PIT) to generate a regular square wave and use this to calculate the input frequency then turn that into the angle. 

in the code:

1. **Memory Locations and Constants:**
   - `timer_value`, `frequency_low`, and `frequency_high`: These memory locations are used to store the timer/counter value, low byte of frequency, and high byte of frequency, respectively.
   - `pit_control_port` and `pit_data_port`: These constants represent the I/O port addresses for the PIT control and data ports.
   - `frequency_divisor`: This constant is used as the divisor for frequency calculation.
   - `clock_frequency`: This constant represents the clock frequency (assumed to be 4 MHz in this example).

2. **Program Start (`org 0x0000`):**
   - The program starts executing at the address `0x0000`.

3. **Main Program (`main`):**
   - The program initializes the PIT counter 0 in square wave mode by sending appropriate control values to the PIT control port.
   - The initial count value for counter 0 is set to 0xFFFF (maximum 16-bit value), effectively setting the timer to count down from its maximum value.
   - The program then reads the timer/counter value from the PIT and stores it in the memory locations `timer_value` and `timer_value + 1`.
   - It calculates the frequency by dividing the clock frequency by the `frequency_divisor` using a subroutine called `div16`.
   - The calculated frequency's low and high bytes are stored in `frequency_low` and `frequency_high` memory locations.
   - The program then halts, waiting for further instructions.

4. **Subroutine `div16`:**
   - This subroutine divides the 16-bit number in HL by 16 (equivalent to a right shift by 4 bits, which is a division by 2^4 = 16).
   - It performs 4 shifts (one shift per bit in the low byte) using the `srl` instruction and the `djnz` loop to repeat the shift 4 times.

5. **Additional Subroutines and Code:**
   - There's a comment suggesting that additional subroutines and code can be added at this point in the program.

6. **End of Program (`end main`):**
   - This marks the end of the program.

code assumes a certain clock frequency and configuration for the PIT.  with a different clock frequency or configuration, need to adjust the constants and calculations accordingly. Additionally, the code doesn't include the handling of the square wave signal itself; it focuses on initializing the PIT and calculating the frequency based on the timer/counter values. If you want to perform additional tasks with the generated square wave, you'll need to extend the code accordingly.




## another
https://www.electronics-lab.com/ne555-timer-sparks-low-cost-voltage-to-frequency-converter/

![image](https://github.com/SteveJustin1963/tec-SCOPE/assets/58069246/5af4808d-1427-46c5-b78d-ee9c48157a15)





##  stage 2 - pots with cam angle motors
- Two DC motor drives attached to the telescope mount using threaded rod+ nuts= worm drives acting as a swinging cam
- The worm drive and cam radius design will enhance torque control and minimize backlash.
- add 2 switch joy stick to enable manual slewing 
- tec1 also outputs 2 motor on off controls
- optical gate counts rotations from slotted wheel on shaft to workout movement
- calibrate at 0 and 90 degrees, also resistor pots are left on for comparison/ extra calibration 
- motor turns vs angle is non-linear due to the cam action. non-linear interpolation and trigonometry used to calculate angles
- Stellarium gets position data via serial and also can issue movement commands back to motors


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






##  stage 3 - remove pots and cam motos, replace with geared motor drive and rotary encoders
- remove cams, motors and resistor pots 
- replace with reduction geared DC or stepper motors and with rotary encoder
- keep manual slewing and software motor control 


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

 


## Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2 phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2 phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

I need to add a Timing Belt Drive Pulley to its 6mm shaft. will construct a wood wheel and invert a drive pulley with the teeth facing out so the drive pulley can interface with it

![image](https://user-images.githubusercontent.com/58069246/223073009-7ab32520-386b-4db5-81f6-1c0dc1765fc9.png)


## code examples

https://electropeak.com/learn/how-to-interface-a-rotary-encoder-with-arduino/
 

This is an Arduino sketch for reading a rotary encoder with AB 2-phase rectangular orthogonal pulse output. The rotary encoder has two output channels, A and B, which produce pulses that are 90 degrees out of phase with each other. By reading the relative phase of the A and B pulses, the sketch is able to determine the direction of rotation of the encoder.

In the setup function, the sketch sets up the serial communication at a baud rate of 9600 and configures the pin 2 and 3 as inputs with pull-up resistors. It also sets up external interrupts on pin 0 and 1, and assigns the A and B functions to be called when the interrupts are triggered.

In the loop function, the sketch checks if the value of the counter has changed since the last time it was sent over serial. If the counter has changed, it sends the new value over serial.

The A and B functions are called whenever an interrupt is triggered on pin 0 and 1, respectively. These functions check the state of the other input pin to determine the direction of rotation of the encoder. If the other input is low, the counter is incremented (in the case of A) or decremented (in the case of B). If the other input is high, the counter is decremented (in the case of A) or incremented (in the case of B).

##  read the differential and count up or down
code uses the @ operator to read the value of pins 2 and 3 before deciding whether to increment or decrement the counter variable. The @ operator retrieves the value of a memory location (in this case, the value of pins 2 and 3) and pushes it onto the data stack. The IF statement then checks the value of the top item on the data stack (either 0 or 1, depending on the value of the pin) and executes the appropriate branch.

The output of this modified code will be a series of numbers, each representing the current value of the counter variable. The value of counter will be incremented or decremented by 1 each time an interrupt is triggered on port 2 or 3, depending on the differential signal read from those pins.
 


## Iterate
- ASR012 https://www.nve.com/Downloads/ASR012.pdf  mag angle sensor
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

##  References
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



