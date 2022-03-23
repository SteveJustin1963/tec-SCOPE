draft

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)



### A simple version
![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)
- rotary encoders 600 pulse/cycle geared down off telescope turn points
- move by hand, encoders send io to tec-1 as dc pulse phase pairs
- MINT code to convert into map data then into ascii codes
- sent ascii to Stellerium one way serial 
- Stellerium updates map position

### More complex
![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/plan2.png)

- position show on map as above
- map slew to location
- track a position in motion
- joystick slew to a position 
- remote control over modem
 
### Parts 
- IO, https://github.com/SteveJustin1963/tec-IO
- Serial and APU, https://github.com/SteveJustin1963/tec-APUS
- Rotary encoders, addon or inside motor
- Gears, telescope shafts, and maybe in motors to increase torque
- Stepper motors, https://github.com/SteveJustin1963/tec-Stepper-Motor-Control
- camera mount
- photo compositing on pc
- 

### build
- Test with small cardboard mock up
- test with small scope
- test with our old 12.5" Newtonian reflector on the original mounts 


## References
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



## Encoder spec
![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)
- Power source: DC5-24V
- Shaft: 6*13mm/0.23*0.51"
- Size: 38*35.5mm/1.49*1.39"
- Output :AB 2phase output rectangular orthogonal pulse circuit, the output for the NPN open collector output type
- Maximum mechanical speed: 5000 R / min
- Response frequency: 0-20KHz
- Cable length: 1.5 meter
- Notice:AB 2phase output must not be directly connected with VCC, otherwise, will burn the output triode, because different batches, and may not have the terminal
   
### connection:

- Green = A phase, white = B phase, red = Vcc power +, black = V0
### Application:
- Is used to measure the rotational speed, Angle and acceleration of the object and the length measurement
Suitable for intelligent control of various displacement measurement, 
- automatic fixed-length leather automatic guillotine machines, steel cut length control, civil measured height human scale, Students racing robots


### Package included:
- 1 x Encoder Incremental Rotary Encoder
