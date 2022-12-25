

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)

### forward

Az Istenek áldásával and in dedication to my late father, uram Jusztin Istvan (Stephen Justin) Born 1932- RIP 2000, from Kiskunfélegyháza Magyarország Hungary, who later emigrated to Australia in 1962. His passage into astronomy was in the late 1940s, he hand made an 8 inch reflector telescope with his friends. Much later in 1976 when I was 13 years old, I saw a repeating advert in the TV week "Make your own telescope! by Astro Optical at Clarke Lane, Crows Nest, Sydney, NSW, Australia". My dad took interest and 3 years later we finished building a 12.5 inch Fény-Vödör reflector telescope. Id like to acknowledge Kiskunfélegyházi Csillagászati Egyesület; the local astronomy club in Kiskunfélegyháza. Now 42 years later I hope to control this old telescope with a TEC-1, MINT and Stellarium star maps.    


### project
that involves using rotary encoders to read and control a telescope. I wrote some code to convert the encoder data into map data, which I then sent to Stellerium.   




### A simple version

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

- rotary encoders 600 pulse/cycle geared down off telescope turn points
- move by hand, encoders send io to tec-1 as dc pulse phase pairs
- or sent to a counter/latch then read at intervals
- MINT code to convert into map data then into ascii codes
- sent ascii to Stellerium one way serial 
- Stellarium updates map position

### Parts 
- IO, https://github.com/SteveJustin1963/tec-IO
- Serial and APU, https://github.com/SteveJustin1963/tec-APUS
- Rotary encoders and gearing
- DC motors control and gearing, 
- torque compensation / control
- Stepper motors and control
- anti shaking

### Build
- Test with small cardboard mock up
- test with small scope, cheap reflector eg 4 or 6" mirror, about $100 used
- test with my old 12.5" Newtonian reflector that has equatorial mounts and geared controls
- 



### poor mans encoder and motor drive in one, 
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




### Iterate
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

### encoder counter chip
- https://www.usdigital.com/products/accessories/interfaces/ics/





 





## Encoder spec

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)
```
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
   
### connection:

- Green = A phase, white = B phase, red = Vcc power +, black = V0
### Application:
- Is used to measure the rotational speed, Angle and acceleration of the object and the length measurement
Suitable for intelligent control of various displacement measurement, 
- automatic fixed-length leather automatic guillotine machines, steel cut length control, civil measured 
height human scale, Students racing robots


### Package included:
- 1 x Encoder Incremental Rotary Encoder
```


