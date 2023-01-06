

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)


In this project, we will be developing a system to control a telescope using rotary encoders, DC motors, and stepper motors. The rotary encoders will provide precise position data, which will be used to locate the map position or control the movement of the telescope. We will gear down the encoders to increase the resolution of the position data, and we will send this data to a TEC-1 controller using IO. The TEC-1 will use MINT code to convert the pulse data into map data and ASCII codes, which will be sent via a one-way serial connection to Stellarium, a planetarium software. This will allow us to accurately track the movement of the telescope in real-time.

In the second stage of the project, we will incorporate DC motors to control the movement of the telescope, enabling capabilities such as slewing and tracking. We will use the rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. We will also use gearing to increase the precision of the position control and torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability.

Finally, we will incorporate stepper motors to control the movement of the telescope, allowing us to achieve very precise positioning. This will allow us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.





 
In this technical project, we will be developing a system to control a DIY Dobson 200mm F5 telescope in three stages.

## In the first stage, 
we will be using a two-motion mount with altitude (vertical) and azimuth (horizontal) axes. We will attach a resistor pot on each axis point to measure the position of the mount and use a simulated ADC circuit with a RC circuit to read the position of the pots. This data will be sent to a TEC-1 controller, which will use MINT code to convert it into ASCII codes and send it to Stellarium, a planetarium software. Stellarium will move the cross hairs on the map to confirm what we are looking at, and we will need to calibrate the system with a reference star.

## In the second stage, 
we will attach two DC motor drives to the mount using threaded rods and nuts that act as a worm drive. A slotted wheel on the shaft will interrupt an optical switch to count the rotations, and this data will be sent to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. The resistor pots on the mount axes will remain in place to compare measurements. The relationship between the number of turns and the angle of the mount is not linear due to the cam, so we will use code and trigonometry functions to calculate an accurate position. The worm drive and cam radius design also improve torque control and reduce backlash. We will add switches to slew the DC motors to aim at celestial objects, and we will use the TEC-1 to control the motors to track or hold onto celestial objects.

## In the third stage, 
we will remove the cams and replace the DC motors with stepper motors, which will be geared at a 6:1 ratio or higher wheel mounted to the side of the axis. This will allow us to achieve more precise positioning. We will also remove the resistor pots and replace them with rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. With the gearing and software enhancements, we will be able to increase position control and implement torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability. Slewing will be done by software rather than DC switches, allowing us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)



![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)

![image](https://user-images.githubusercontent.com/58069246/210787050-8d2eaeb9-b0c7-4354-a721-ea8eefa45b56.png)

![image](https://user-images.githubusercontent.com/58069246/210787088-6ed89615-bbfc-4401-9106-50af9890bbe9.png)




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

# Astronomy
## arcsecond 
An arcsecond is a unit of angular measurement that is equal to 1/60 of an arcminute and 1/3600 of a degree. In other words, there are 3600 arcseconds in one degree.

Arcseconds are commonly used in astronomy to express small angles or angular separations. For example, the angular diameter of the Moon as seen from the Earth is approximately 0.5 degrees, or 30 arcminutes, or 1800 arcseconds. The resolution of a telescope or other optical instrument is often expressed in arcseconds, with higher numbers indicating better resolution.

One way to think about arcseconds is as a measure of the angle subtended by a small object or feature at a given distance. For example, if you are viewing a distant object through a telescope, the size of the object in arcseconds will depend on its distance from the telescope and the size of the telescope's objective lens or mirror.

To count 1 arcsecond accuracy with a +/- 0.5 arcsecond error, you will need to use a gear ratio that produces a sufficient number of pulses per arcsecond. The required gear ratio will depend on the number of pulses per revolution produced by the encoder and the desired accuracy of the measurement.

To calculate the required gear ratio, you can use the following formula:

gear ratio = (pulses per arcsecond) / (pulses per revolution / 360 degrees)

For example, if the encoder produces 600 pulses per revolution and you want to achieve 1 arcsecond accuracy with a +/- 0.5 arcsecond error, you will need a gear ratio of at least:

gear ratio = (3600 pulses/arcsecond) / (600 pulses/revolution / 360 degrees) = 6

This means that you will need to down gear the encoder by a factor of at least 6:1 in order to achieve the desired accuracy.

Keep in mind that this is a minimum requirement, and you may need to use a higher gear ratio or implement additional measures (such as calibration or error correction) in order to achieve the desired accuracy in your specific application.

## Sun
The angular diameter of the Sun as seen from the Earth is approximately 0.53 degrees, or 31.8 arcminutes, or 1911 arcseconds. This means that if you were to view the Sun through a telescope from the Earth, it would appear to be 1911 arcseconds in size.

Keep in mind that the angular diameter of the Sun can vary slightly due to the elliptical shape of the Earth's orbit around the Sun. At its closest approach to the Earth (called perihelion), the Sun's angular diameter is slightly larger than at its farthest point (called aphelion). However, the difference in angular diameter is relatively small and is not noticeable to the naked eye.

It is important to use caution when observing the Sun, as looking directly at the Sun can cause serious eye damage. It is recommended to use special solar filters or indirect viewing methods when observing the Sun.

## Mars
The angular diameter of Mars as seen from the Earth can vary significantly due to the elliptical shape of Mars' orbit around the Sun. At its closest approach to the Earth (called perihelion), Mars' angular diameter can be as large as 25.1 arcseconds. At its farthest point (called aphelion), Mars' angular diameter can be as small as 3.5 arcseconds.

The average angular diameter of Mars as seen from the Earth is approximately 15 arcseconds. However, this value can vary depending on the specific location of Mars in its orbit and the position of the Earth relative to Mars.

It is important to note that the angular diameter of Mars as seen from the Earth is much smaller than the angular diameter of the Sun. This means that Mars appears much smaller in the sky and is more difficult to observe than the Sun. The best time to observe Mars is when it is at its closest approach to the Earth, which occurs approximately once every two years. At this time, Mars will appear larger and brighter in the sky and will be easier to see with a telescope or other optical instrument.

## Earth
The speed of Earth's rotation, or the angular velocity of the Earth, is approximately 0.00417 degrees per second. This is equivalent to approximately 15 arcseconds per second. The Earth's rotational speed is not constant, and can vary slightly due to a number of factors such as the gravitational pull of the Moon and the Sun, the distribution of mass within the Earth, and the Earth's axial tilt. However, the overall rotational speed of the Earth is relatively constant over time, and the variations are small enough that they are not noticeable to most people. The Earth's rotational speed determines the length of a day, which is defined as the time it takes for the Earth to make one complete rotation on its axis. The length of a day is approximately 24 hours, or 86400 seconds. This means that the Earth's rotational speed is approximately 15 arcseconds per second, or 86400 seconds per day.

## Field of view (FOV) 
is a measure of the amount of sky that is visible through a telescope or other optical instrument at a given moment. It is usually expressed in degrees, arcminutes, or arcseconds. The field of view is determined by a combination of factors, including the focal length of the telescope, the size of the eyepiece, and the eyepiece's field of view. A larger field of view allows you to see a wider area of the sky at once, while a smaller field of view allows you to see a narrower, more detailed area.
In general, telescopes with shorter focal lengths (such as refractors) have wider fields of view, while telescopes with longer focal lengths (such as reflectors) have narrower fields of view. Eyepieces also have an impact on the field of view. Eyepieces with a wider field of view will produce a wider field of view in the telescope, while eyepieces with a narrower field of view will produce a narrower field of view. The field of view is an important consideration for astronomers, as it determines how much of the sky can be seen at once. It is also important for photographers, as it determines the size of the area that can be captured in a single photograph.
To find the angular field of view through a telescope eyepiece, you need to know the eyepiece's field of view and the telescope's focal ratio. The angular field of view of an eyepiece is usually marked on the eyepiece itself or in the manufacturer's specifications. If you don't have this information, you can estimate the angular field of view by using the formula:
```Angular field of view (in arcseconds) = (Eyepiece field of view in degrees) / (206265 / Focal ratio)```
For a 35mm eyepiece with a field of view of 50 degrees and a telescope with a focal ratio of f/5, the angular field of view would be approximately:
```(50 degrees) / (206265 / 5) = 2.46 arcseconds```
Keep in mind that this is just an estimate, and the actual field of view may vary slightly depending on the eyepiece and telescope.

## Alpha Centauri 
is a star system located about 4.37 light-years from Earth, so it is not visible to the naked eye. However, if you were able to see Alpha Centauri through a telescope, the angular size of the system would depend on the distance between the Earth and Alpha Centauri, as well as the size of the telescope's field of view. Alpha Centauri A is a star located in the Alpha Centauri star system, which is about 4.37 light-years from Earth. If you were using a telescope with a field of view of 2.46 arcseconds and the distance between the Earth and Alpha Centauri A was 4.37 light-years, the size of Alpha Centauri A in the eyepiece would be approximately 0.003 inches.

To calculate this, you would use the formula:
```Size in eyepiece (in inches) = (Angular size of object in arcseconds) / (Field of view in arcseconds)```
Plugging in the values for Alpha Centauri A and the eyepiece field of view, you get:
```(0.007 arcseconds) / (2.46 arcseconds) = 0.003 inches = 0.0762 millimeters```
Keep in mind that this is just an estimate, and the actual size of Alpha Centauri A in the eyepiece may vary depending on the specifics of the telescope and eyepiece.

## The magnification 
of a telescope is a measure of how much an object is enlarged by the telescope. A higher magnification allows you to see more detail in an object, but it also makes the image dimmer and reduces the field of view.

The magnification of a telescope is determined by the focal length of the telescope and the focal length of the eyepiece. It is calculated using the formula:
```Magnification = (Telescope focal length) / (Eyepiece focal length)```
For example, if you have a telescope with a focal length of 1000mm and an eyepiece with a focal length of 25mm, the magnification would be ```40x (1000 / 25 = 40).```
To see detail in an object, you will need a magnification that is high enough to resolve the details of the object. The amount of magnification you need will depend on the size and distance of the object, as well as the quality of the telescope and eyepiece. In general, a magnification of 50-100x is sufficient to see detail in most objects, but some objects may require higher magnifications. It is important to note that using too much magnification can actually make the image worse, as the image can become distorted and blurry due to atmospheric conditions and the limitations of the telescope and eyepiece.



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
