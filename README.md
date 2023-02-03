# A TEC-1 Controlled Telescope

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)  


## Dedication 

With the blessings of the God and in dedication to my late father, Istvan Jusztin (Stephen Justin), who was born in 1932 in Kiskunfélegyháza, Hungary and passed away in 2000. He emigrated to Australia in 1962. He developed a passion for astronomy in the late 1940s, when he and his friends built their own 8-inch reflector telescope by hand. In 1976, when I was 13 years old, I saw an advertisement on in the back of TV-Week that caught my father's attention. It was for Astro Optical in Crows Nest, Sydney, Australia, and it promoted the opportunity to "Make your own telescope!" My father and I decided to take on this challenge and spent the next 3 years building a 12.5-inch Fény-Vödör reflector telescope. I would like to acknowledge the Kiskunfélegyházi Csillagászati Egyesület, the local astronomy club in Kiskunfélegyháza, for their support and guidance back then. Now, 42 years later, I hope to control this old telescope with a TEC-1 single board computer and with MINT and ASM code, and with Stellarium star maps and serial code software, to continue my father's legacy in the field of astronomy which I too now share.




In this project, we will be developing a system to control a DIY Dobson 200mm F5 telescope in three stages. Here's my baby $35 off Gumtree. Needs an eyepiece but has a spare 200mm mirror.

![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)


## In the first stage, 
of this project, and all of them, we will keep the Dobsons two-motion mount on the altitude (vertical) and azimuth (horizontal) axes. The telescope will still need to be moved by hand. We will attach a 2 resistor pots, one to each axis pivot point, to measure the position of the mount. We will use a 555 IC circuit to convert this value into a variable frequency of square pulses, which will be sent to the TEC-1s IO board addon. Using MINT code, we will count the pulses and wrt to the angle we can interpolate the angle of the scope. Also we can compare the counts to previous count intervals to determine the direction of movement of the mount. This data can be converted into ASCII codes and sent to Stellarium, a planetarium software, via a serial line. Stellarium use this and compute the positon of the cross hairs on the map. After calibration with a reference star, this can confirm what we are looking at.

![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

![image](https://user-images.githubusercontent.com/58069246/211299181-19e803e9-cc92-4894-b500-6ed76fe9ce4f.png)

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




0000000000000000000000000000000000000000000000000000000000

# Random stuff

## Alternative to counting pulses from variable R, use a cap, C
- https://www.instructables.com/Simple-Capaitance-Meter/
- convert code to c `count-scm.c`

we need a 555 cct to measure cap vale and turn it into pulses, then in the same way feed it into tec1 like the variable r pulses.

## encoder counter chip
- https://www.usdigital.com/products/accessories/interfaces/ics/

## Convert a DVD/CD-ROM lens mechanism into an eyepiece 
- https://www.instructables.com/How-to-make-a-eyepiece-for-telescope/
![image](https://user-images.githubusercontent.com/58069246/211810295-89b8375f-4bbb-4390-8c8d-e2ea61d7648e.png)
![image](https://user-images.githubusercontent.com/58069246/211817426-755ec785-08d5-4dfc-9d82-04b2c97553e2.png)
![image](https://user-images.githubusercontent.com/58069246/211817479-856412fa-585b-47e9-81f4-f2d0382ea6da.png)
![image](https://user-images.githubusercontent.com/58069246/211818405-93306d5c-035f-47c6-898a-ecb0deb14043.png)


## A lens used in a CD-ROM 
(Compact Disc Read-Only Memory) drive has several important characteristics that are designed to optimize the performance of the drive. Some of these characteristics include:
- Numerical Aperture (NA): The numerical aperture of a lens is a measure of its ability to gather light and focus it onto the disc. A higher numerical aperture means that the lens can gather more light and produce a brighter, more focused image of the disc.
- Focal Length: The focal length of a lens is the distance between the lens and the point where the light is focused. A shorter focal length allows the lens to have a wider field of view, while a longer focal length produces a narrower field of view but can magnify the image more.
- Wavefront Distortion: The lens in a CD-ROM drive must be designed to minimize wavefront distortion, which is the deviation of the shape of a wavefront from a perfect spherical shape. Wavefront distortion can cause the light to be focused incorrectly, resulting in a blurry or distorted image of the disc.
- Spherical Aberration: Spherical aberration is an optical effect that occurs when light passing through a lens is not brought to a perfect focus. This effect can cause a loss of sharpness in the image and make it harder for the drive to read the disc correctly.
- Astigmatism: Astigmatism is another optical effect that occurs when the lens is not able to focus all of the light on a single point. This effect can cause distortion of the image, making it harder for the drive to read the disc correctly.
- Chromatic Aberration: Chromatic Aberration is an effect caused by the lens failing to focus all the different colours of light at the same point. This can cause a colour fringe around the object.

These characteristics are essential to the proper function of a CD-ROM drive. A lens with a high numerical aperture, a short focal length, and minimal wavefront distortion, spherical aberration, astigmatism and chromatic aberration will produce a clear, sharp image of the disc that can be read accurately by the drive.

## The focal length of a lens used in a CD-ROM drive 
is typically in the range of `6 mm to 15 mm`. This is a relatively short focal length which allows the lens to have a wide field of view and to be able to focus on a large area of the disc at once. This is important for a CD-ROM drive because it needs to be able to scan the entire surface of the disc quickly in order to read the data stored on it. The lens in a CD-ROM drive is typically a double-Gauss design which combines two simple lens groups arranged in a negative-positive-negative configuration, this lens design is known to be compact and well suited for wide-angle applications. The `numerical aperture` of the lens used in a CD-ROM drive is generally around `0.45`, this allows it to gather enough light to produce a bright, focused image of the disc. It is important to note that some of the characteristics of the lens such as its focal length and numerical aperture might vary depending on the exact design of the drive, as well as on the production batch, but in general the values stated are within the expected ranges for a CD-ROM drive lens. 

## The focal length of the lens used in a DVD drive 
is similar to that of a CD-ROM drive, typically ranging from 6 mm to 15 mm. Like CD-ROM drives, DVD drives need to be able to scan a wide area of the disc quickly in order to read the data stored on it, so a wide field of view is important. However, DVD drives use a different technology to read the disc called multi-beam optics. This technology uses multiple, closely spaced beams to scan the disc, allowing the drive to read multiple data tracks at once. This requires a more complex and expensive lens assembly than what is used in CD-ROM drives. The `numerical aperture` of the lens used in a DVD drive is typically around `0.6 - 0.7` which is a `bit higher than CD-ROM drive`. This allows it to `gather more light` and produce a brighter, more focused image of the disc. This is important because DVD disc have much more data and smaller pits than CD and they have to be read accurately.
It is also important to note that DVD drives use a laser diode to read the disc data, and the wavelength of the laser diode used in DVD drives (655nm) is different from the wavelength of the laser diode used in CD-ROM drives (780nm), and that also affects the design of the lens used in DVD drives.

## The Apparent Field of View (AFoV) 
of an eyepiece is the angular size of the sky, in degrees, that appears in the eyepiece. It can be calculated using the formula:
```
AFoV = 2 * arcsin (NA / (2 * f / D))
where:
NA is the numerical aperture of the eyepiece
f is the focal ratio of the telescope (f/D)
D is the diameter of the objective lens or primary mirror (in this case, 200 mm)
For a telescope with a focal ratio of f/5 and a 200 mm primary mirror, the focal length is:
f = D / f/D = 200mm / 5 = 40mm

- For a numerical aperture of 0.45:
AFoV = 2 * arcsin (0.45 / (2 * 40 / 200)) = 2 * arcsin (0.45 / 0.4) = 2 * arcsin (1.125) = approximately 46.0 degrees
- For a numerical aperture of 0.7
AFoV = 2 * arcsin (0.7 / (2 * 40 / 200)) = 2 * arcsin (0.7 / 0.4) = 2 * arcsin (1.75) = approximately 66.7 degrees
```

It's worth noting that this formula is an approximation, and that actual apparent field of view can vary due to different factors such as lens quality, manufacturing variations and lens coatings. The formula only gives an estimate of the field of view, that can be considered as a reference. The Apparent Field of View (AFoV) of an eyepiece can vary depending on the design and quality of the eyepiece, as well as the focal ratio and aperture of the telescope it is used with. Generally, professional telescope eyepieces have a wider apparent field of view than standard eyepieces.
For a telescope with an f/5 focal ratio and a 200 mm primary mirror, an AFoV of 46-67 degrees, as calculated previously, can be considered within the normal range for an eyepiece with a standard design and quality. For comparison, a professional eyepiece used with this telescope can have an AFoV of around 60-70 degrees or more, depending on the specific eyepiece and the manufacturers design.
However, as previously stated, AFoV varies between different eyepieces designs, some eyepieces designs can have a wider FOV, such as the wide-angle eyepieces, others can have a smaller FOV but with better sharpness, such as the orthoscopic eyepieces.
Keep in mind that the apparent field of view is just one of the many factors that determine the quality of an eyepiece and a telescope. Other factors such as sharpness, distortion, and eye relief are also important to consider.

## Photo Detector Array, 
also known as a photodetector array or optical pick-up, is a device used in CD-ROM drives to read the data stored on a compact disc. The array typically consists of a laser diode, a collimator lens, and a photodiode array. The laser diode emits a focused beam of light onto the surface of the disc. The collimator lens is used to ensure that the light beam is parallel and has a consistent width as it travels across the disc. The photodiode array is located behind the collimator lens and is responsible for detecting the reflected light from the disc. The photodiode array is typically made up of several individual photodiodes, each with its own lens and detector. These individual photodiodes are arranged in a line or pattern and work together to detect the light reflected from the disc. As the disc spins, the beam of light scans across the disc, and the photodiode array detects the variations in the reflected light caused by the data stored on the disc. The data is then sent to the drive's electronics, where it is decoded and sent to the computer. 

## Specifications of a photodiode array 
can vary depending on the manufacturer and the type of CD-ROM drive it is used in. Some common specifications include:
- Number of photodiodes: The array typically has several individual photodiodes, with a higher number allowing for a more precise reading of the data on the disc.
- Sensitivity: This is a measure of how well the array can detect light. A higher sensitivity means that the array is able to detect weaker reflections from the disc, which is important for reading data on lower quality discs.
Wavelength range: The wavelength range refers to the range of light frequencies that the array can detect. For CD-ROM drives, this is typically in the near infrared range (around 780nm).
- Spectral response: This refers to the range of light frequencies that the array can detect and is typically given as a graph.
The size of the array: this depends on the size of the lens used in the CD-ROM drive, some arrays are bigger than others to cover a larger area of the disc.
- Some other specifications that may be provided by the manufacturer include the dark current, noise, and linearity of the array. These specifications can affect the performance of the array and the overall quality of the data read from the disc.

It is possible that a photodiode array used in a CD-ROM drive could be repurposed for photon measurements in astronomy, but it would likely not be the optimal solution. Photodiode arrays used in CD-ROM drives are designed to read data stored on compact discs, which requires a different set of specifications and characteristics than those needed for photon measurements in astronomy.

For example, in astronomy, the measurement should be performed in a low light condition, where a high sensitivity and low noise are important specifications. However, CD-ROM drives are usually optimized for reading data in relatively bright environments.
Additionally, the wavelength range and spectral response of a photodiode array used in a CD-ROM drive may not be well-suited for the range of light frequencies typically encountered in astronomical measurements.

In summary, a photodiode array used in a CD-ROM drive would not be an ideal solution for photon measurements in astronomy, but it's not impossible to repurpose it and use it for this specific application with the proper modifications and calibration. it would be advisable to use a dedicated device that is specifically designed for the task of photon measurements.

There are several alternatives to a photodiode array that can be used for astronomical measurements, depending on the specific requirements of the application. Some examples include:
- CCD (charge-coupled device): A CCD is a type of image sensor that can be used to capture images of stars and galaxies. They are widely used in astronomical telescopes and can be sensitive to a wide range of wavelengths, from the ultraviolet to the infrared.
- CMOS (complementary metal-oxide-semiconductor): CMOS sensors are similar to CCDs but they are typically less expensive, they consume less power, and they can operate at higher speeds, which makes them suitable for high-speed imaging and low-light conditions.
- Photomultiplier tubes (PMT): PMTs are used to detect very weak light signals, and are commonly used in low-light-level applications like in spectroscopy, timing measurements, and scintillation counting. They are very sensitive to light and can detect single photons.
- APD (Avalanche Photodiodes): APDs are similar to photodiodes, but they are designed to detect weak light signals by amplifying the electrical current generated by the incoming photons. They are also used for low-light-level applications like spectroscopy, Timing measurements, and scintillation counting.
- ICCD (intensified charge-coupled device): An ICCD is a type of image sensor that uses a device called an intensifier to amplify the weak light signals that are captured by the CCD. This makes them well-suited for low-light-level applications, and for capturing fast-moving objects.
- EMCCD (electron multiplying CCD): An EMCCD is similar to an ICCD, but it uses a different technique to amplify the light signals captured by the CCD. This results in even higher sensitivity and low noise, making them ideal for very low-light-level applications.
- sCMOS (Scientific complementary metal-oxide-semiconductor): A scientific CMOS sensor is a new sensor technology that is designed to provide high resolution, low noise, and high frame rate in a single package, which makes them well suited for scientific applications like Astronomy.
- SPAD (Single Photon Avalanche Diode): SPADs are similar to APDs, but they are designed to detect single photons with a high efficiency, very high sensitivity and low noise, which makes them well suited for applications like Time-of-flight imaging, LIDAR and Spectroscopy.
- PIN photodiode. The Osram BPW34

Each one has its own unique characteristics, capabilities, and limitations, with different advantages and disadvantages, so the choice of sensor depends on the specific requirements of the application, such as the wavelength range and sensitivity needed, the speed of data acquisition, and the budget available.

## CMOS sensor 
would likely be the easiest to experiment with and use with a low power computer.
CMOS sensors are relatively inexpensive, easy to interface with, and consume less power than CCDs or ICCDs. Additionally, they can operate at high speeds and can be easily integrated into a variety of devices. This makes them well-suited for use in low power, portable or embedded systems.
Another reason why CMOS sensors could be the easiest to work with, is because of its low noise and high resolution, which provides a good image quality even at low light levels. The software required to operate the sensor can be relatively simple, and it could be run on a low power computer.
The best option would depend on the specific requirements of the application, such as the wavelength range and sensitivity needed, the speed of data acquisition, and the budget. 

## OV9726
- S/N ratio: 37 dB
- dynamic range: 69 dB
- sensitivity: 1300 mV/(lux-sec)

## OV2640 
- S/N ratio	40dB
- Dynamic Range	50dB
- Sensitivity	0.6V/Lux-sec

The most important feature for astronomy is sensitivity, as this determines how much light the sensor can detect. In this comparison, the OV9726 has a higher sensitivity of 1300 mV/(lux-sec) compared to the OV2640 which has a sensitivity of 0.6V/Lux-sec. Therefore, the OV9726 would be the better choice for star gazing performance.


## ISO 
is a measure of the sensitivity of a digital camera's sensor to light. A higher ISO value usually means the camera can capture more light, resulting in a brighter image. However, increasing the ISO can also cause more noise in the image, and thus can be a tradeoff when taking photos in low-light conditions. In astronomy, CMOS sensors are often used in astrophotography. The increased sensitivity of the sensor to light allows for much longer exposures, which is necessary to capture faint astronomical objects. Increasing the ISO can reduce the amount of time needed to capture a good image, but it can also introduce more noise. So it is important to find the right balance between exposure time and ISO in order to get the best image, and every camera increases noise in a different way. Some slowly ramp it up, others make a big jump after a particular setting.

## PIN photodiode. Osram BPW34
- https://www.farnell.com/datasheets/2046123.pdf
![image](https://user-images.githubusercontent.com/58069246/211954551-80435a5a-b534-43ae-8664-3f3e462bf82b.png)


The BPW34 is a type of photodiode that can be used for astronomical experiments. Based on the provided specifications, it has several characteristics that make it suitable for astronomical optical experiments:
- High breakdown voltage (V(BR)): With a minimum breakdown voltage of 60V, the BPW34 can handle high electrical stress, making it suitable for use in high-voltage applications such as astronomical experiments.
- Low reverse dark current: The reverse dark current is the current that flows through the photodiode when it is not exposed to light. With a minimum reverse dark current of 2nA, the BPW34 has a low dark current, which means that it is less likely to produce noise in the images.
- High sensitivity: The BPW34 has a high sensitivity of 0.6V/Lux-sec, which means that it can detect weak light signals, making it suitable for low-light-level applications such as astronomical experiments.
- Wide wavelength range: The BPW34 has a wide wavelength range of 430 to 1100nm, which means that it can detect light across a wide range of frequencies, making it suitable for capturing images of objects that emit light in different wavelength ranges.
- High noise equivalent power: The BPW34 has a low noise equivalent power of 4 x 10^-14 W/√Hz, which means that it is able to detect very weak light signals with a low noise level.

On the other hand, it also has a couple of limitations. One of them is the small range of the angle of half sensitivity of ±65 degrees, which means that this photodiode does not have a wide field of view, limiting its use for capturing images of large areas of the sky. Additionally, it has a relatively high power consumption.

Overall, the BPW34 appears to be a suitable choice for astronomical experiments, as it has a high breakdown voltage, low reverse dark current, high sensitivity, and a wide wavelength range, which allows for capturing images of objects in different wavelength ranges. However, the small range of angle of half sensitivity and high power consumption are important factors that should be considered.

## Single Star, variable light
experiment 
- Choose a distant star that is known to be variable, such as 
  - Algol, https://en.wikipedia.org/wiki/Algol
  - Mira, https://en.wikipedia.org/wiki/Mira
  - Epsilon Aurigae, https://en.wikipedia.org/wiki/Epsilon_Aurigae
- Mount the BPW34 photodiode on a telescope, and aim it at the chosen star.
- Use a filter that limits the light to the wavelength range of 430 to 1100nm, to match the BPW34 sensitivity range.
- Use a data acquisition system to record the electrical current flowing through the photodiode as a function of time.
- Take data for a period of several months, at regular intervals (for example, once a week).
- Analyze the data to determine the variability of the light emitted by the star. Look for patterns in the data that indicate regular changes in brightness, such as periodic variability.
- Compare the results to previous studies of the same star to see if there are any differences in the variability.
- Repeat the experiment using other filters to study the variability of the star's light at different wavelengths.

This experiment allows you to study the variability of the light emitted by a distant star, which can provide insights into the physical processes taking place within the star. Additionally, it allows you to test the ability of the BPW34 photodiode to detect weak light signals over a wide wavelength range and compare the results with other studies.

## Double Star System 
experiment 
- Choose a double star system that is known to be variable, such as 
  - Castor, https://en.wikipedia.org/wiki/Castor_(star)
  - Mizar, https://en.wikipedia.org/wiki/Mizar
- Mount the BPW34 photodiode on a telescope, and aim it at the chosen double star system.
- Use a filter that limits the light to the wavelength range of 430 to 1100nm, to match the BPW34 sensitivity range.
- Use a data acquisition system to record the electrical current flowing through the photodiode as a function of time.
- Take data for a period of several months, at regular intervals (for example, once a week).
- Analyze the data to determine the variability of the light emitted by the double star system. Look for patterns in the data that indicate regular changes in brightness, such as periodic variability.
- Compare the data for each star individually, in order to understand the contribution of each star to the variability.
- Repeat the experiment using different filters to study the variability of the double star system at different wavelengths.
- Compare the results of this experiment with previous studies of the same double star system.

This experiment allows you to study the variability of light emitted by a double star system to gain insight into the physical processes taking place within the stars, while testing the ability of the BPW34 photodiode to detect weak light signals over a wide wavelength range and comparing the results with other studies, instruments such as CCD camera, and even with the naked eye to get a better understanding of the performance of the sensor and the influence of the other factors in the final results.


## Telescope Eyepiece 
is a component that is placed at the end of a telescope's focuser and is responsible for magnifying the image formed by the telescope's objective lens or primary mirror. The eyepiece works by `increasing the apparent size of the image` that the objective lens or mirror forms, which makes the image appear closer and larger.

The `magnification` of an eyepiece is determined by the `ratio of the telescope's focal length to the focal length of the eyepiece`. 
For example, 
- an eyepiece with a focal length of 10 mm 
- used in a telescope with a focal length of 1000 mm 
- would have a magnification of 100x (1000/10). 
This means that an object viewed through the telescope would appear `100 times closer` than it would with the naked eye.

## The field of view (FOV) of an eyepiece 
is another important characteristic. It is defined as the angular size of the area of the sky that can be seen through the eyepiece. The FOV is determined by the design of the eyepiece and its optics. Eyepieces with a wide field of view will allow you to see more of the sky at once, but the image may appear less sharp than with an eyepiece with a narrower field of view. A common measure of FOV is the `Apparent Field of View (AFoV)`, is the angular size of the sky, in degrees, that appears in the eyepiece.

An eyepiece is typically composed of multiple lens elements, some eyepiece designs include 
- Plössl, 
- Kellner 
- and Orthoscopic. 
Each of these designs have their own advantages and disadvantages, such as level of sharpness, distortion, or field of view.

An interesting feature of some eyepiece designs is the ability to adjust the `eye relief`, which is the distance between the eyepiece and the observer's eye for which the full field of view is visible. This is useful for eyeglass wearers, or anyone who has trouble getting their eye close enough to the eyepiece. The construction of an eyepiece has an important impact on the visual performance, some eyepieces are made of high-quality glass, while others are made of plastic. Eyepieces that are made of high-quality glass will typically have better visual performance, and will be more durable, but they may be more expensive.
Finally, there are also a variety of eyepiece accessories such as barlow lens, or a T-adapter for eyepiece projection photography, that can enhance the functionality of a telescope eyepiece.



## Resolution of interpolation of code
There are a few different ways to make the code more accurate to say 0.1 degrees, depending on the specifics of the application and the data that the code is working with. Here are a few options:
- Increase the resolution of the lookup table: The code uses a lookup table to map ADC values to angle. One way to increase the accuracy of the code is to increase the resolution of the lookup table by adding more entries. This will allow the code to more accurately interpolate between values in the table.
- Use a more accurate interpolation method: The code uses linear interpolation to estimate the angle from the ADC value. Using a more accurate interpolation method such as a polynomial or spline interpolation can improve the accuracy of the code.
- Use more decimal places: The code currently uses a single decimal place for the final temperature reading. Using more decimal places in the final temperature reading will increase the accuracy of the code.
- Use a cct with higher accuracy: The accuracy of the final angle reading is limited by the accuracy of the sensor. Using a more sensitive angle pulse cct with higher accuracy will improve the overall accuracy of the code.

This code uses the same basic structure as the previous example, but with different values in the lookup table and a different variable name. Here, the ADC values are used to interpolate an angle between 0 and 18, with a resolution of 0.1 degrees. Please note that, the angle_lookup table values should be replaced with actual data. `interpol_1.c`

```
float interpolate_angle(int adc_val)
{
    // Lookup table with higher resolution
    int Lookup[19] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    // Angle values corresponding to the above ADC values
    float angle_lookup[19] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0};
    for (int i = 18; i > 0 ; i--) 
    {
    if (Lookup[i - 1] > adc_val) 
    {
        // Using linear interpolation
        float angle = angle_lookup[i-1] - (((float)(adc_val - Lookup[i-1]) / (float)(Lookup[i-1] - Lookup[i])) * (angle_lookup[i-1]-angle_lookup[i]));
        //Rounding off to 0.1 degree
        angle = round(angle * 10) / 10;
        return angle;
    }
    }
}
```
Here's an example of how the code could be modified to interpolate an angle between 80 and 130 degrees, using a pair of 8-bit integers to store the result and the lookup table. In this example, I changed the range of the angle from 0-18 to 80-130, the lookup tables are also updated to the new range, and I used a pair of 8-bit unsigned integers to store the result and the lookup table. The resolution of the interpolation remains the same, 0.1 degree.
Keep in mind that as you're using 8-bit integers, you will be limited by the range of the integers(0-255) which you are using. If the value of angle exceeds 130 then the function will return the maximum value i.e. 255. `interpol_2.c`
```
#include <stdint.h>

uint8_t interpolate_angle(uint8_t adc_val)
{
    // Lookup table with higher resolution
    uint8_t lookup_adc[51] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
    // Angle values corresponding to the above ADC values
    uint8_t lookup_angle[51] = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130};

    for (int i = 50; i > 0; i--)
    {
        if (lookup_adc[i - 1] > adc_val)
        {
            // Using linear interpolation
            uint8_t angle = lookup_angle[i - 1] - (((float)(adc_val - lookup_adc[i - 1]) / (float)(lookup_adc[i - 1] - lookup_adc[i])) * (lookup_angle[i - 1] - lookup_angle[i]));
            return angle;
        }
    }
}
```
## Polynomial interpolation 
is a method of estimating the value of a function at a certain point, given a set of data points that the function passes through. The basic idea behind polynomial interpolation is to fit a polynomial function of degree n (where n is the number of data points) through the n data points, such that the polynomial function passes through all of the data points. The polynomial function can then be used to estimate the value of the function at any point within the range of the data points. For example, if we have a set of data points (x0, y0), (x1, y1), ..., (xn, yn), we can fit a polynomial function of degree n through these data points. The polynomial function can be represented by the equation:
```
p(x) = a0 + a1*x + a2*x^2 + ... + an*x^n
```
where a0, a1, ..., an are the coefficients of the polynomial that need to be determined.

There are different methods to find the coefficients of polynomial like

- Newton's Divided Differences : Here you'll use the concept of recursive formula to find the polynomial coefficients.
- Least Squares : Here you will try to minimize the difference between the function f(x) and the polynomial function p(x) by minimizing the sum of the square of the differences between them.

Once the coefficients are found, the polynomial function can be used to estimate the value of the function at any point within the range of the data points. One of the advantage of polynomial interpolation is that the polynomial function can be used to estimate the value of the function at any point, even outside the range of the data points, although the accuracy of the estimation will be reduced outside the range. One of the major disadvantage of polynomial interpolation is Runge's phenomenon. Where a polynomial that is a good fit to a set of data at the center of the data will produce large oscillations (Runge's phenomenon) near the boundaries. It is generally more pronounced when the degree of the polynomial is high. Thus polynomial interpolation is a powerful method for estimating the value of a function at a certain point, given a set of data points. It can be applied to both equally and unequally spaced data points and can give accurate results if the function is well represented by a polynomial of low degree. However, it can be sensitive to the distribution of the data points and can produce large oscillations near the boundaries.

To code this, use the Newton's Divided Differences method for polynomial interpolation. The function takes an ADC value as input, then it uses the x and y arrays to store the data points. The arrays are passed to the function, which uses them to compute the polynomial coefficients and estimate the angle at the input ADC value. It loops through the arrays, the outer loop is used to calculate the divided difference of the data points and the inner loop is used to calculate the interpolated value. Still you'll need to replace the x and y arrays with your own data points and also make sure that your polynomial degree doesn't exceed the amount of data points you have. As the data points are known and fixed it is also called as a Lagrange Interpolation.
`interpol_poly_new_1.c`

```
#include <cmath>

float interpolate_angle(float adc_val)
{
    // Data points
    float x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    float y[] = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};
    int n = sizeof(x) / sizeof(x[0]);
    float a[n];
    for (int i = 0; i < n; i++)
        a[i] = y[i];
    for (int j = 1; j < n; j++)
        for (int i = 0; i < n - j; i++)
            a[i] = (a[i + 1] - a[i]) / (x[i + j] - x[i]);
    float result = a[0];
    float term = 1;
    for (int i = 0; i < n - 1; i++) {
        term = term * (adc_val - x[i]);
        result = result + a[i + 1] * term;
    }
    return result;
}
```
## Spline interpolation 
is a method for estimating the value of a function at a certain point, given a set of data points that the function passes through. The basic idea behind spline interpolation is to fit a smooth piecewise polynomial function through the data points, such that the polynomial function is continuous, and its first and second derivatives are also continuous at the points where the polynomials are joined together.

In contrast to polynomial interpolation, where a single polynomial function is fit through all the data points, spline interpolation uses multiple polynomial functions, each fitting a subset of the data points. The polynomials are joined together at the data points, creating a smooth curve that passes through all the data points.

There are several types of spline interpolation, such as:
- Linear spline interpolation : In this method, a straight line is used to interpolate between each pair of data points. It is simple to use but the interpolated function does not have any smoothness property.
- Quadratic spline interpolation: Here, Quadratic polynomials are used to interpolate between data points. The interpolated function has a continuous first derivative, but not a continuous second derivative.
- Cubic spline interpolation: Cubic polynomials are used to interpolate between the data points. The interpolated function has a continuous first and second derivative, providing a smooth curve that passes through all the data points.
- Natural spline : In this type of spline, the second derivative of the polynomial function is set to zero at the endpoints. It tends to provide a more smooth curve compared to the cubic spline.
- Cubic spline interpolation is the most widely used method. It is widely used in computer graphics, numerical analysis, and other fields where a smooth interpolation is desired. One of the major advantage of spline interpolation is that it provides a smooth curve that passes through all the data points, so it preserves the local features of the data.

Spline interpolation has one major disadvantage, which is that it requires more computation to find the polynomial functions than polynomial interpolation, especially when the number of data points is large. Thus, spline interpolation is a powerful method for estimating the value of a function at a certain point, given a set of data points. It provides a smooth curve that passes through all the data points, preserving the local features of the data. However, it requires more computation than polynomial interpolation, especially when the number of data points is large.

To code the natural cubic spline interpolation method. The function takes an ADC value as input and uses it to estimate the angle. It utilizes x and y arrays to store the data points. It first finds the differences between consecutive data points and then the slopes of the tangents at the data points using this difference and the y values. Then it uses these values to calculate the second derivative at the data points using the tridiagonal matrix algorithm. Finally, it uses this second derivative to interpolate the angle at the input ADC value. And you'll need to replace the x and y arrays with your own data points to use it. Also, note that the spline interpolation techniques like this are relatively more complex and computationally expensive as compared to polynomial interpolation or linear interpolation methods.
`interpol_nat_cubic_spline_1.c`
```
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

float interpolate_angle(float adc_val)
{
    // Data points
    vector<float> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    vector<float> y = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};
    int n = x.size();
    vector<float> h, b, d, alpha, c(n), l(n), u(n), z(n);
    h.resize(n);
    b.resize(n);
    d.resize(n);
    alpha.resize(n);
    for (int i = 0; i < n - 1; i++)
        h[i] = x[i + 1] - x[i];
    for (int i = 1; i < n - 1; i++)
        alpha[i] = 3 * (y[i + 1] - y[i]) / h[i] - 3 * (y[i] - y[i - 1]) / h[i - 1];
    l[0] = 1;
    u[0] = 0;
    z[0] = 0;
    for (int i = 1; i < n - 1; i++) {
        l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * u[i - 1];
        u[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }
    l[n - 1] = 1;
    c[n - 1] = 0;
    for (int i = n - 2; i >= 0; i--) {
        c[i] = z[i] - u[i] * c[i + 1];
        b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (c[i + 1] + 2 * c[i]) / 3;
        d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
    }
    int k = 0;
    for (int i = 0; i < n - 1; i++) {
        if (adc_val >= x[i] && adc_val <= x[i + 1]) {
            k = i;
            break;
        }
    }
    float result = y[k] + (adc_val - x[k]) * b[k] + (adc_val - x[k]) * (adc_val - x[k]) * c[k] + (adc_val - x[k]) * (adc_val - x[k]) * (adc_val - x[k]) * d[k];
    return result;
}
```





# Astronomy
## Arcsecond 
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

## The Rayleigh limit 
is the smallest size of an object that can be resolved by a telescope or other imaging system. It is determined by the wavelength of light being used and the diameter of the telescope's aperture. The formula for the Rayleigh limit is:
`θ = 1.22 * λ / D` where θ is the angular resolution, λ is the wavelength of light, and D is the diameter of the telescope's aperture.
The smaller the value of θ, the greater the resolving power of the telescope. However, the Rayleigh limit is often not the limiting factor in determining a telescope's resolution, as other factors such as atmospheric turbulence and the quality of the optics can also come into play.

## eg 
The angular resolution (θ) for a telescope with an aperture of 200mm and an f-ratio of f/5 can be calculated using the Rayleigh limit formula:
`θ = 1.22 * λ / D` where λ is the wavelength of light, and D is the diameter of the telescope's aperture.
For visible light with a wavelength of 550 nanometers:
```
θ = 1.22 * 550 * 10^-9 m / (200 * 10^-3 m)
θ = 2.2 * 10^-6 radians
```
So the angular resolution of the telescope is 2.2 * 10^-6 radians. This is the smallest angle at which two point sources can be distinguished as separate in the telescope.

Please note that this is a theoretical resolution, real-world resolution is limited by factors such as atmospheric turbulence and the quality of the optics, which can lower the resolution.

## convert the angular resolution in radians to arc seconds, 
you can use the following conversion factor:
`1 radian = (180/π) * 3600 arc seconds` So,
`2.2 * 10^-6 radians = (2.2 * 10^-6) * (180/π) * 3600 arc seconds = 0.39 arc seconds`. 
So the angular resolution of the telescope with an aperture of 200mm and an f-ratio of f/5 is `0.39 arc seconds`. This means that two point sources can be distinguished as separate if they are separated by an angle of 0.39 arc seconds or greater in the telescope.


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

## Astigmatism 
is a type of optical aberration that occurs when the image of a point object is not focused to a single point, but instead appears as a line or elongated shape. It is caused by the different curvatures of the lens or mirror in a telescope or camera lens. It is most commonly caused by an asymmetry in the curvature of the lens or mirror, which causes light to be focused differently in different directions. Astigmatism can be corrected by using a special lens called a "corrective lens" which compensates for the asymmetry of the primary lens or mirror.
Astigmatism is often present in combination with other types of aberrations, such as spherical aberration and chromatic aberration. It can be corrected with a combination of lenses, mirrors, or other optical elements in a telescope or camera lens. It also can be corrected by using special software during image processing. Astigmatism can be measured by using a special device called a "star test" which consists of shining a point source of light through the lens and observing its shape on a screen or detector.

## Astigmatism is a common problem in reflector telescopes, 
which use mirrors instead of lenses to gather and focus light. In a reflector telescope, astigmatism is caused by the mirror being deformed or not having a perfect parabolic shape, which can cause the light to be focused differently in different directions. This results in an elongated or distorted image. Astigmatism can also be caused by misalignment of the mirror in the telescope's optical train, which can cause the light to be focused differently in different directions.
Astigmatism can be corrected in a reflector telescope by adjusting the mirror cell, which holds the mirror in place, to ensure that the mirror is properly aligned and has the correct shape. It can also be corrected by using a special lens or mirror called a "corrective lens" which compensates for the asymmetry of the primary mirror. Additionally, special software can be used during image processing to correct the astigmatism caused by the imperfect shape of the mirror.
It's worth mentioning that Astigmatism is not unique to reflectors, refractor telescopes also can have it, but it occurs less frequently in refractors because the lenses are less likely to deform, and also the lens is less likely to be misaligned in the optical train.

## Astigmatism test in a reflector telescope:
- Star test: This is the most common method for testing astigmatism in a reflector telescope. It involves shining a point source of light, such as a bright star, through the telescope and observing the shape of the image on a screen or detector. If the image is elongated or distorted, it is likely that the telescope has astigmatism.
- Knife-edge test: This test involves placing a thin blade or wire across the telescope's aperture and observing the shape of the shadow it casts. If the shadow is distorted or elongated, it is likely that the telescope has astigmatism.
- Ronchi test: This test involves using a special device called a Ronchi grating, which consists of a series of parallel lines, to observe the image of the telescope's aperture. If the image is distorted or elongated, it is likely that the telescope has astigmatism.
- Foucault test: This test involves shining a point source of light through the telescope and observing the shape of the image on a screen or detector. It is similar to the star test, but it also uses a knife-edge to measure the shape of the shadow that the mirror casts.

It's worth noting that astigmatism testing is best done in the dark, under good seeing conditions, and with a bright star as the target. Also, it is important to keep in mind that astigmatism is not the only aberration that can cause the image to be distorted, so it's best to check for other types of aberrations as well.

https://www.skyatnightmagazine.com/advice/how-to-star-test-a-telescope/




## Lucky-Imaging 
- https://github.com/SteveJustin1963/tec-SCOPE-Lucky-Imaging


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






## Activity log

## 25.12.22
Last week, I purchased an old DIY 8-inch reflector telescope for a project I am working on. The telescope cost $30 and came with a spare 8-inch mirror, but did not include an eyepiece.

This will be the first test for my astronomy project and I am excited to explore the celestial objects that can be viewed through it. I am looking forward to learning more about telescopes and the technology used to observe the night sky. I hope to use the skills I gain from this project to further my understanding of astronomy and share my knowledge with others in the field.

spec 8 inch mirror is 200mm, focal length is 1m therefor its 1000/200=5 or F5

## 22.9.22 
I am currently investigating different methods for counting rotary encoder movement, including the use of a counter with a flip-flop circuit. I am considering the pros and cons of using interrupts versus other methods, and also exploring the potential use of DC motors versus stepper motors.

In my research, I have looked into the ASCII codes for the LX200 telescope and considered various IO port options. I am working to determine the best solution for my project, taking into account factors such as accuracy, efficiency, and cost.

## 17.9.22 
I recycled a broken foot massage machine for the 24V DC motor and worm drive. Now I need another one, or I maybe try using a stepper motor instead. I'm considering trying both options

## 23.3.2022 
ordered 2 rotary encoders, 600p/r

## 1.3.2022 
chat with Craig Jones: I am designing a telescope control system that uses serial communication and interrupts to manage incoming data streams. I am considering using the Motorola 6850 Asynchronous Communications Interface Adapter (ACIA) and implementing circular buffered serial interrupts in my design. I will need at least three serial ports for this system: one for the ACIA, one for the connection to the laptop running Stellarium, and one for the MINT code to send data to the ACIA.

The system will receive two pairs of phase-shifted DC pulses, which will be used to track the movement of the telescope. These pulses will be counted and converted into data that is used by the MINT code to convert to astronomy coordinates and ASCII codes. These codes will be uploaded to my laptop running the Stellarium software via a second serial port, allowing me to move the crosshairs on the star map and track celestial objects.

I am concerned about the complexity of reading the four data ports, as slight movement can result in large pulse trains. However, I expect that the final ASCII data update will be small once the pulses have been converted.

I have access to a 12-inch reflector telescope and smaller reflectors, and I am interested in using these telescopes for observation. In the future, I hope to use the MINT code to add stepper or DC motors to track objects, add a joystick, and other additional features.

It is not clear what I mean by the "?" symbol reading a serial port or how to implement a second bitbang serial port. It would be helpful to have more context or clarification on this part of the discussion.

I always intended to add buffered serial interrupts to the MINT code, and the first version was designed to run on a basic TEC-1 system with 2k of memory. I have completed the buffered serial interrupt code, but it has not yet been incorporated into the MINT code. It may be necessary to increase the size of the MINT code to handle both the APU and serial buffered communications. I will leave the implementation of these features to John.

## 1.2.2019 
chat Peter Forth:
I am interested in using the Planetarium software and the NodeMCU ESP8266 for an astronomy project. I am a member of the Win32forth group, which includes astronomers such as Michel, Iruata Souza, and Roland Herrera. Michel Jean, a professor from Canada, has started his own version of Stellarium on ForthWin.

I own an Arduino Mega2560, which I have used to control a Mearm robot and other projects. I believe it would be easy to use this device to control a stepper motor drive for a 2-axis telescope mount.

There are many people in the group who are interested in astronomy and creating graphs related to celestial objects. I am excited to work on this project and learn more about astronomy and the use of technology in this field.

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
