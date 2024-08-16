integrating a TEC1-D controlled system with your 8-inch Dobson telescope at F5:
![image](https://github.com/user-attachments/assets/71fbb606-9846-47ff-9faa-8f20c86fc306)


![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)



- what can we see ? eye piece 5 mm and 8 inch mirror at F5 gives gives 8 degree field of view thats around 0.6 arc seconds between two points of light.


1. **Mechanical and Hardware Setup:**
   - Mounts
   - Motors and encoders
   - Track telescope position using an optical gate

2. **Basic Telescope Control:**
   - Calibration
   - Slewing
   - Tracking to compensate for Earth's rotation or moon's movement

3. **Advanced Telescope Control:**
   - Go-to functionality (slew to specific object's coordinates, user database)
   - Alignment assistance in aligning telescope with celestial objects

4. **Communication and Interfacing:**
   - Starmap communication with Stellarium
   - Bitbang to starmap
   - Enable remote control via TCS

5. **Data Handling and Display:**
   - Interpolation
   - Display values

6. **Special Features and Enhancements:**
   - Camera control for long-exposure astrophotography






![image](https://github.com/user-attachments/assets/175f6647-cef5-44bb-8ab4-b49f28b1b4a6)





## reading angles
### manual
angle measurements.
on the axis of mount a reader and turn that into numbers 
 mechanical compass convert magnetic north to true north, Sydney needs about 12 added (magnetic declination was 12.83°E in 2020 with 0.04° annual change).
 
 and protractor,
 or electronic equivalents. 
 
 


## display angle
- read anglel data from encode
- convert to angles
- display on tec1 seven seg display
- we have 6 digit xxxx xx

code
- press A then azimuth and alt to 1 degree accuracy is given ie 0299 23 meaning 299 degs 23 deg
- or press `+` then alt is displayed to higher precision ie 0299 44 meaning 299 degs 44 minutes
- press + again we more precision ie 0044 51 meaning 44 minutes and 51 seconds
- or press `-` then azimuth is displayed to higher precision ie 0023 16 meaning 23 degs 16 minutes
- press - again we more precision ie 0016 22 meaning 16 minutes and 22 seconds  
 

 
Storing precision in groups of integers, often referred to as fixed-point arithmetic, can be a way to maintain better accuracy than using plain 16-bit integers for certain calculations, especially when dealing with fractional values. Fixed-point arithmetic allows you to represent fractional numbers with a fixed number of integer bits and fractional bits. This approach can help you maintain precision while using integer-based data types.

 
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







