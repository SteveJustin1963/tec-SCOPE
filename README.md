# A TEC-1 Controlled Telescope
![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)

- develop a system to read and control a cheap Dobson 200mm F5 telescope 
- in three stages.





##  stage1 , 
1. Retaining Dobson's 2-axis mount: Keep the original Dobson's 2-axis mount, allowing manual movement along the altitude and azimuth axes.
2. Attaching resistor pots: Attach two resistor pots to each axis pivot point to measure the approximate position of the mount.
3. Converting potentiometer readings: Use a 555+ circuit to convert the potentiometer readings into a variable frequency of square pulses.
4. Sending pulses to I/O port: Send the pulses to an I/O port on the TEC1 for pulse counting per second.
5. Counting pulse frequencies: Utilize pulse frequencies in the low and sub kHz range suitable for counting through the I/O port.
6. Interpolation and scaling: Employ interpolation techniques using a scaling factor to determine the angle based on the pulse frequency.
7. Determining the direction of movement: Compare pulse counts to previous intervals to determine the direction of movement.
8. Converting movement data to serial data: Convert the movement data into serial data using ASCII codes.
9. Integration with Stellarium: Use the ASCII codes to compute the position of the crosshairs on the star map using Stellarium, an astronomical software.
10. Calibration with a reference star: Perform calibration with a reference star to confirm the accuracy of the observations.



![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

 

circuit simulator https://tinyurl.com/2p8fdmws

make the 220 value the pot

![image](https://user-images.githubusercontent.com/58069246/220818068-d6cbff32-57e3-4e6c-9fc0-8369b4c0e593.png)

## another
https://www.electronics-lab.com/ne555-timer-sparks-low-cost-voltage-to-frequency-converter/

![image](https://github.com/SteveJustin1963/tec-SCOPE/assets/58069246/5af4808d-1427-46c5-b78d-ee9c48157a15)





##  stage 2, 
- Two DC motor drives will be attached to the mount using threaded rods and nuts as worm drives.
- An optical switch will be used to count the rotations of a slotted wheel attached to the motor shaft.
- The rotation data will be sent to the TEC-1 for conversion into ASCII codes using MINT.
- The ASCII codes will be sent to Stellarium for processing.
- The resistor pots on the mount axes will be retained for comparison purposes.
- The relationship between the number of turns and the mount angle is non-linear due to the cam.
- Code and trigonometry functions will be used to accurately calculate the mount's position.
- The worm drive and cam radius design will enhance torque control and minimize backlash.
- Switches will be added to enable slewing of the DC motors for aiming at celestial objects.
- The TEC-1 will control the motors to track or maintain the position of celestial objects.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






##  stage 3, 
we will remove the cams and replace the DC motors with stepper motors, which will be geared at a 6:1 ratio or higher, with a wheel mounted to the side of the axis. This will allow us to achieve more precise positioning. We will also remove the resistor pots and replace them with rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. With the gearing and software enhancements, we will be able to increase position control and implement torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability. Slewing will be done by software rather than DC switches, allowing us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

 


## Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

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


# Activity log

## 1.2.2019 
chat with Peter Forth: I am interested in using the Planetarium software and the NodeMCU ESP8266 for an astronomy project. I am a member of the Win32forth group, which includes astronomers such as Michel, Iruata Souza, and Roland Herrera. Michel Jean, a professor from Canada, has started his own version of Stellarium on ForthWin. I own an Arduino Mega2560, which I have used to control a Mearm robot and other projects. I believe it would be easy to use this device to control a stepper motor drive for a 2-axis telescope mount.

There are many people in the group who are interested in astronomy and creating graphs related to celestial objects. I am excited to work on this project and learn more about astronomy and the use of technology in this field.

## 1.3.2022 
chat with Craig Jones: I am designing a telescope control system that uses serial communication and interrupts to manage incoming data streams. I am considering using the Motorola 6850 Asynchronous Communications Interface Adapter (ACIA) and implementing circular buffered serial interrupts in my design. I will need at least three serial ports for this system: one for the ACIA, one for the connection to the laptop running Stellarium, and one for the MINT code to send data to the ACIA.

The system will receive two pairs of phase-shifted DC pulses, which will be used to track the movement of the telescope. These pulses will be counted and converted into data that is used by the MINT code to convert to astronomy coordinates and ASCII codes. These codes will be uploaded to my laptop running the Stellarium software via a second serial port, allowing me to move the crosshairs on the star map and track celestial objects.

I am concerned about the complexity of reading the four data ports, as slight movement can result in large pulse trains. However, I expect that the final ASCII data update will be small once the pulses have been converted.

I have access to a 12-inch reflector telescope and smaller reflectors, and I am interested in using these telescopes for observation. In the future, I hope to use the MINT code to add stepper or DC motors to track objects, add a joystick, and other additional features.

It is not clear what I mean by the "?" symbol reading a serial port or how to implement a second bitbang serial port. It would be helpful to have more context or clarification on this part of the discussion.

I always intended to add buffered serial interrupts to the MINT code, and the first version was designed to run on a basic TEC-1 system with 2k of memory. I have completed the buffered serial interrupt code, but it has not yet been incorporated into the MINT code. It may be necessary to increase the size of the MINT code to handle both the APU and serial buffered communications. I will leave the implementation of these features to John.

## 23.3.2022 
ordered 2 rotary encoders, 600p/r

## 17.9.2022 
I recycled a broken foot massage machine for the 24V DC motor and worm drive. Now I need another one, or I maybe try using a stepper motor instead. I'm considering trying both options

## 22.9.2022 
I am currently investigating different methods for counting rotary encoder movement, including the use of a counter with a flip-flop circuit. I am considering the pros and cons of using interrupts versus other methods, and also exploring the potential use of DC motors versus stepper motors.

In my research, I have looked into the ASCII codes for the LX200 telescope and considered various IO port options. I am working to determine the best solution for my project, taking into account factors such as accuracy, efficiency, and cost.


## 25.12.22
Last week, I purchased an old DIY 8-inch reflector telescope for a project I am working on. The telescope cost $30 and came with a spare 8-inch mirror, but did not include an eyepiece.

This will be the first test for my astronomy project and I am excited to explore the celestial objects that can be viewed through it. I am looking forward to learning more about telescopes and the technology used to observe the night sky. I hope to use the skills I gain from this project to further my understanding of astronomy and share my knowledge with others in the field.

spec 8 inch mirror is 200mm, focal length is 1m therefor its 1000/200=5 or F5



## 1.06.2023
Got another two massage machines and hacked out the  24v motors and proper worm drives

## 3.07.2023
Looking full moon, fantastic detail, brightness hurts the eye, put camera body try photograph, focal plane lower than camera
