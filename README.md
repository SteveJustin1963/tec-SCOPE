# A TEC-1 Controlled Telescope

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)  






In this project, we will be developing a system to control a DIY Dobson 200mm F5 telescope in three stages. Here's my $35 Gumtree find to play with.

![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)


## In the first stage, 
We will keep the Dobson's 2 axis mount with manual movement on the altitude (vertical) and azimuth (horizontal) axes. 

Two resistor pots will be attached on each axis pivot point to measure approx position. A 555 + circuit will convert this into a variable frequency of square pulses, which will be sent to a io port on the tec1 to be counted per second. With the pulse frequency in the low and sub khz counting the pulses through the io should be ok. Using a scaling factor we interpolate to scopes angle from the freq. Additionally, we can compare the counts to previous intervals to determine the direction of movement. We can then converted this to serial data ASCII codes that Stellariumcan use to compute the position of the crosshairs on the map. After calibration with a reference star, we can confirm our observations.

![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

 

circuit simulator https://tinyurl.com/2p8fdmws

make the 220 value the pot

![image](https://user-images.githubusercontent.com/58069246/220818068-d6cbff32-57e3-4e6c-9fc0-8369b4c0e593.png)


 




## code var-R-code
 
 

 
 
 


## In the second stage, 
we will attach two DC motor drives to the mount using threaded rods and nuts that act as a worm drive. A slotted wheel on the shaft will interrupt an optical switch to count the rotations, and this data will be sent to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. The resistor pots on the mount axes will remain in place to compare measurements. The relationship between the number of turns and the angle of the mount is not linear due to the cam, so we will use code and trigonometry functions to calculate an accurate position. The worm drive and cam radius design also improve torque control and reduce backlash. We will add switches to slew the DC motors to aim at celestial objects, and we will use the TEC-1 to control the motors to track or hold onto celestial objects.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






## In the third stage, 
we will remove the cams and replace the DC motors with stepper motors, which will be geared at a 6:1 ratio or higher, whith a wheel mounted to the side of the axis. This will allow us to achieve more precise positioning. We will also remove the resistor pots and replace them with rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. With the gearing and software enhancements, we will be able to increase position control and implement torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability. Slewing will be done by software rather than DC switches, allowing us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

 


## Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

i need to add a Timing Belt Drive Pulley to its 6mm shaft. will contrstruct a wood wheel and invert a drive pully with the teeth facing out so the drive pully can interface with it

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
- track a position in motion. compensate for earths rotation. satellite tracking different problem
- joystick slew scope to new positions 
- remote control over modem, using defined protocols, TCS - Telescope Control System
- camera mount + camera
- photo compositing on pc
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

