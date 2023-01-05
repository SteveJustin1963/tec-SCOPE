

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)





## Project 
![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

The rotary encoders on our telescope provide precise position data that allows us to locate the map position or control the movement of the telescope. These encoders generate 600 pulses per cycle, which can be geared down to increase the number of pulses per arc length, corresponding to the movement of the telescope. In the first stage, we will manually move the telescope and have the encoders send IO data to the TEC-1 as DC pulse phase pairs. Alternatively, we can send the data to a counter/latch and read it at regular intervals, which is a better option as it avoids overwhelming the TEC-1 with too much data. The MINT code we will develop will convert this data into map data and ASCII codes, which will then be sent via a one-way serial connection to Stellarium. This will update the map position in Stellarium in real-time, allowing us to accurately track the movement of the telescope. When we look through the telescope, we should see the same star map on the laptop.

In the second stage, we can incorporate motors to control the movement of the telescope, which will give us more capabilities such as slewing, tracking, and more. We can use the rotary encoders to measure the position of the motors and send the data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. This will allow us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.

## Parts 
- IO, https://github.com/SteveJustin1963/tec-IO
- Serial and APU, https://github.com/SteveJustin1963/tec-APUS
- Rotary encoders and gearing
- DC motors control and gearing, 
- torque compensation / control
- Stepper motors and control
- anti shaking


## poor mans encoder and motor drive in one, 
- with a two motions mount, altitude (vertical) and azimuth (horizontal)
- have a resistor pot on the two shafts
- as well 2x dc motor drives via a threaded rod thru a nut on a cam arms to the x and y. 
- a simple slotted wheel on the shaft interrupts a optical switch to count the rotations, ie 10 slots. 
- as its a cam, the relationship between turns and angle is not linear. 
- so we read the pot via an adc 
- then do sine function times the rotation counts to derive an accurate position
- the cam also improves torque and back lash control
- with Equatorial mounts and properly aligned can be aimed at a celestial object and guided manually or one control/motor.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)

![image](https://user-images.githubusercontent.com/58069246/210787050-8d2eaeb9-b0c7-4354-a721-ea8eefa45b56.png)

![image](https://user-images.githubusercontent.com/58069246/210787088-6ed89615-bbfc-4401-9106-50af9890bbe9.png)




##  Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

- Green = A phase, white = B phase, red = Vcc power +, black = V0
- Power source: DC5-24V
- Shaft: 6*13mm/0.23*0.51"
- Size: 38*35.5mm/1.49*1.39"
- Output :AB 2phase output rectangular orthogonal pulse circuit, the output for the NPN open 
collector output type
- Maximum mechanical speed: 5000 R / min
- Response frequency: 0-20KHz
- Cable length: 1.5 meter
- Notice:AB 2phase output must not be directly connected with VCC, otherwise, will burn the output triode, 
because different batches, and may not have the terminal
- Is used to measure the rotational speed, Angle and acceleration of the object and the length measurement
Suitable for intelligent control of various displacement measurement, 


https://electropeak.com/learn/how-to-interface-a-rotary-encoder-with-arduino/

```code1.c```

This is an Arduino sketch for reading a rotary encoder with AB 2-phase rectangular orthogonal pulse output. The rotary encoder has two output channels, A and B, which produce pulses that are 90 degrees out of phase with each other. By reading the relative phase of the A and B pulses, the sketch is able to determine the direction of rotation of the encoder.

In the setup function, the sketch sets up the serial communication at a baud rate of 9600 and configures the pin 2 and 3 as inputs with pull-up resistors. It also sets up external interrupts on pin 0 and 1, and assigns the A and B functions to be called when the interrupts are triggered.

In the loop function, the sketch checks if the value of the counter has changed since the last time it was sent over serial. If the counter has changed, it sends the new value over serial.

The A and B functions are called whenever an interrupt is triggered on pin 0 and 1, respectively. These functions check the state of the other input pin to determine the direction of rotation of the encoder. If the other input is low, the counter is incremented (in the case of A) or decremented (in the case of B). If the other input is high, the counter is decremented (in the case of A) or incremented (in the case of B).

##  read the differential and count up or down
code uses the @ operator to read the value of pins 2 and 3 before deciding whether to increment or decrement the counter variable. The @ operator retrieves the value of a memory location (in this case, the value of pins 2 and 3) and pushes it onto the data stack. The IF statement then checks the value of the top item on the data stack (either 0 or 1, depending on the value of the pin) and executes the appropriate branch.

The output of this modified code will be a series of numbers, each representing the current value of the counter variable. The value of counter will be incremented or decremented by 1 each time an interrupt is triggered on port 2 or 3, depending on the differential signal read from those pins.
```count-ab.f```



## encoder counter chip
- https://www.usdigital.com/products/accessories/interfaces/ics/




## Iterate
- from a map location slew scope to that position, issues x.200 commands that tec1 turns into x an y and then into motor control
- track a position in motion. compensate for earths rotation. satellite tracking different problem
- joystick slew scope to new positions 
- remote control over modem, using defined protocols, TCS - Telescope Control System
- camera mount + camera
- photo compositing on pc
- MINT code with and without APU help
- 7 Congruent Circles In A Regular Hexagon, use microwave oven glass.
![circle-32_42926_lg](https://user-images.githubusercontent.com/58069246/191688868-d8030041-259c-4365-8445-d773df714613.gif)


### References
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
