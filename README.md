# A TEC-1 Controlled Telescope

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)  






In this project, we will be developing a system to control a DIY Dobson 200mm F5 telescope in three stages. Here's my baby $35 off Gumtree. 

![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)


## In the first stage, 
of this project, and all of them, we will keep the Dobsons two-motion mount on the altitude (vertical) and azimuth (horizontal) axes. The telescope will still need to be moved by hand. We will attach a 2 resistor pots, one to each axis pivot point, to measure the position of the mount. We will use a 555 IC circuit to convert this value into a variable frequency of square pulses, which will be sent to the TEC-1s IO board addon. Using MINT code, we will count the pulses and wrt to the angle we can interpolate the angle of the scope. Also we can compare the counts to previous count intervals to determine the direction of movement of the mount. This data can be converted into ASCII codes and sent to Stellarium, a planetarium software, via a serial line. Stellarium use this and compute the positon of the cross hairs on the map. After calibration with a reference star, this can confirm what we are looking at.

![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

![image](https://user-images.githubusercontent.com/58069246/211299181-19e803e9-cc92-4894-b500-6ed76fe9ce4f.png)


## use a counter chip
https://www.google.com/search?q=74HC590&rlz=1C1FKPE_en-GBAU984AU984&sourceid=chrome&ie=UTF-8




The code will do a range of things, the first being to calibrate angle vs pulses. it will go like this;
## calibrate
1. Press "Go" 
2. The word “CAL A” for  calibrate appears on the 7 segment screen
3. Push the telescope to altitude 80 degrees 
4. Press A
5. IO port 3 counts pulses for 3 seconds from the circuit (around 200 pulses per second) and saves the value
6. Word “CAL B” for  calibrate appears on the 7 segment screen
7. Push the telescope right over to far size to position 160 degrees
8. Press B
9. IO port 3 counts pulses for 3 seconds from the circuit (around 600 pulses per second) and saves the value
10. Use the slope of the line between points A and B to interpolate the angle based on the measured frequency of pulses
11. Take into account the direction of movement (increasing or decreasing) to get the correct result

```count-555.f```

This code defines two Forth words: read-io-port and count-pulses. read-io-port is the same as in the previous example and reads the value from an I/O port at a specified address. count-pulses takes a single input, the address of the I/O port, and counts the number of pulses on the port for 3 seconds. It stores the result in a variable called A. It then returns the final value of A. You can use count-pulses as follows: ```port-address count-pulses .```
This will count the number of pulses on the I/O port at the specified address for 3 seconds and print the result to the screen.






## In the second stage, 
we will attach two DC motor drives to the mount using threaded rods and nuts that act as a worm drive. A slotted wheel on the shaft will interrupt an optical switch to count the rotations, and this data will be sent to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. The resistor pots on the mount axes will remain in place to compare measurements. The relationship between the number of turns and the angle of the mount is not linear due to the cam, so we will use code and trigonometry functions to calculate an accurate position. The worm drive and cam radius design also improve torque control and reduce backlash. We will add switches to slew the DC motors to aim at celestial objects, and we will use the TEC-1 to control the motors to track or hold onto celestial objects.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






## In the third stage, 
we will remove the cams and replace the DC motors with stepper motors, which will be geared at a 6:1 ratio or higher wheel mounted to the side of the axis. This will allow us to achieve more precise positioning. We will also remove the resistor pots and replace them with rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. With the gearing and software enhancements, we will be able to increase position control and implement torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability. Slewing will be done by software rather than DC switches, allowing us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

 


## Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

## code examples

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


