
# Activity log

## 12-9-23
![image](https://github.com/SteveJustin1963/tec-SCOPE/assets/58069246/7a677f95-0dfd-49b4-a4b5-bffa2d479829)



## 11-9-23
this is for step 3. thinking ahead. altitude (vertical) and azimuth (horizontal) rotate shaft mockup, alt control need to
sit in 4 point cog roller bed. the motor work drive i salvaged needs to be mod so worm is on end of shaft, not is in middle. this will 
conflict with mirror box. so need to go on outside

### belt and cog options
- https://www.temu.com/au/powge-gt2-2mgt-2m-2gt-rf-open-ended-synchronous-timing-belt-width-6-9-10-15mm-rubber-low-dust-low-vibration-2gt-6-2gt-9-2gt-10-2gt-15-3d-printer-g-601099519708613.html?
- https://www.temu.com/au/1-2pcs-set-gt2-timing-pulley-20-teeth-and-60-teeth-aperture-5mm-aluminum-timing-pulley-with-2-pieces-of-200mm-wide-6mm-timing-belt-and-1-piece-of-hexagon-wrench-g-601099512632540.html?



## 9-9-23
motor worm drive and shaft, done

## 17.8-2023
Considering Stainless Steel Worm Tin Bronze Worm Gear Wear 1:90 Reduction Ratio Large Reduction

 




## 3.07.2023
Looking full moon, fantastic detail, brightness hurts the eye, put camera body try photograph, focal plane lower than camera, another project

## 1.06.2023
Got another two massage machines and hacked out the 24v motors and proper worm drives. Test runs down to 3v and still strong draw 1 amp

## 25.12.22
Last week, I purchased an old DIY 8-inch reflector telescope for a project I am working on. The telescope cost $30 and came with a spare 8-inch mirror, but did not include an eyepiece.

This will be the first test for my astronomy project and I am excited to explore the celestial objects that can be viewed through it. I am looking forward to learning more about telescopes and the technology used to observe the night sky. I hope to use the skills I gain from this project to further my understanding of astronomy and share my knowledge with others in the field.

spec 8 inch mirror is 200mm, focal length is 1m therefor its 1000/200=5 or F5

## 22.9.2022 
I am currently investigating different methods for counting rotary encoder movement, including the use of a counter with a flip-flop circuit. I am considering the pros and cons of using interrupts versus other methods, and also exploring the potential use of DC motors versus stepper motors.

In my research, I have looked into the ASCII codes for the LX200 telescope and considered various IO port options. I am working to determine the best solution for my project, taking into account factors such as accuracy, efficiency, and cost.

## 17.9.2022 
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
chat with Peter Forth: I am interested in using the Planetarium software and the NodeMCU ESP8266 for an astronomy project. I am a member of the Win32forth group, which includes astronomers such as Michel, Iruata Souza, and Roland Herrera. Michel Jean, a professor from Canada, has started his own version of Stellarium on ForthWin. I own an Arduino Mega2560, which I have used to control a Mearm robot and other projects. I believe it would be easy to use this device to control a stepper motor drive for a 2-axis telescope mount.

There are many people in the group who are interested in astronomy and creating graphs related to celestial objects. I am excited to work on this project and learn more about astronomy and the use of technology in this field.





