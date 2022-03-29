draft

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)



### A simple version

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

- rotary encoders 600 pulse/cycle geared down off telescope turn points
- move by hand, encoders send io to tec-1 as dc pulse phase pairs
- MINT code to convert into map data then into ascii codes
- sent ascii to Stellerium one way serial 
- Stellerium updates map position

### Parts 
- IO, https://github.com/SteveJustin1963/tec-IO
- Serial and APU, https://github.com/SteveJustin1963/tec-APUS
- Rotary encoders and gearing
- DC motors control and gearing, 
- torque compensation / control
- Stepper motors and control
- anti shaking

### build
- Test with small cardboard mock up
- test with small scope
- test with our old 12.5" Newtonian reflector on the original mounts 
- MINT code with and without APU help
- 





## iterate
- from a map location slew scope to that position, issues x.200 commands that tec1 turns into x an y and then into motor control
- track a position in motion. compensate for earths rotation. satellite tracking different problem
- joystick slew scope to new positions 
- remote control over modem, using defined protocols, TCS - Telescope Control System
- camera mount + camera
- photo compositing on pc

## References
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


