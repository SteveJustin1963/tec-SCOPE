

# tec-SCOPE
TEC-1 controlled telescope using Forth, tec-APUS and Stellarium.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)

## Abstract
Forth, TEC-1, Z80, Telescope-Control


## Dedication

Az Istenek áldásával

In dedication to my late father, uram, Jusztin Istvan (Stephen Justin) 1932-2000, and his passage into astronomy back in Kiskunfélegyháza Magyarország (Hungary). He made his own telescope by hand, an 8 inch reflector with his friends. He served his country and flew the MIG-15's and also graduated in mechanical engineering and Tool-making. He emigrated to Australia in 1962 and by 1976 when I was 13 years old, after seeing a TV week advertisement "Make your own telescope! ... Astro Optical, Clarke Lane, Crows Nest, Sydney, NSW, Australia", I convinced my father for us to make a telescope. Three years later we proudly finished our 12.5 inch reflector telescope, called the "Fény-Vödör". 

I also proudly acknowledge "Kiskunfélegyházi Csillagászati Egyesület". 

## Introduction 
My love of the 1983 TEC-1 and ye old Forth code, its time to make old things work again. Now with planetarium software such as Stellarium, I hope to drive and control my old 1975 telescope, to find and explore stars again in a new way.


## Observe and Question 
"The Forth programming language was invented by Charles Moore for control of telescopes in observatories using small computers (Moore 1980). Let's try Firth of Forth by John Hardy http://github.com/jhlagado/firth on the TEC-1 to controll my telescope.

## Theory - Testable
A TEC-1 under Forth control can drive a small telescope and simultaneously interface with planetarium software such as Stellarium. 

## Prediction
"It will work but will not be easy".   

## Method 
1. run Firth on the TEC-1
2. write forth control code add ASM code
  * timer
  * position send / find; and to Stellarium
  * tracking 
  * remote control, modem, radio or tcp/udp socket
3 Test with small cardboard mock up
4. next with 12.5" Newtonian reflector, 

## Parts 
* IO, https://github.com/SteveJustin1963/tec-IO
* Serial and APU, https://github.com/SteveJustin1963/tec-APUS
* Rotary encoders, addon or inside motor
* Gears, telescope shafts, and maybe in motors to increase torque
* Stepper motors, https://github.com/SteveJustin1963/tec-Stepper-Motor-Control
* camera mounted
* photo compositing

## Report, figures, tables

## Results

## Discuss objectively, scientific significance 

## Conclusion 

## Acknowledgements
* Chuck Moore
* John Hardy, Ken Stone
* Peter Forth
PF; Your work with astronomy and forth is fantastic ! You can play with my little "planetarium" and add features if you wish. I want to make a Celestial Sphere later and project the Stars on it. And probably later steer my Telescope through wifi, with nodemcu esp8266. https://github.com/PeterForth/Planetarium Please, write a bit of your  astronomy interest and Forth interest in the Win32forth group, as a presentation of you ! So the other members will see your activity and interest  and respond and want to be friends too... we have many Astronomers in that group ! they will love to hear from you  https://www.facebook.com/peter.forth.583; I like very much to program in Win32forth. and also other forths too. I also like to help new programmers with questions around win32forth. Any case you need me you can contact me. Please tell me a bit about your interests in Astronomy and Forth !

## References
https://www.facebook.com/groups/623556744820045

https://www.facebook.com/groups/2225595150855239

https://www.facebook.com/Kiskunf%C3%A9legyh%C3%A1zi-Csillag%C3%A1szati-Egyes%C3%BClet-124849427661821/

http://stellarium.org/

https://www.hackster.io/neil-pollard/sky-finder-a-lego-alexa-gadget-to-discover-the-night-sky-879e46

https://www.facebook.com/rDUINOScope/

https://www.facebook.com/profile.php?id=150875201997704&ref=br_rs

https://create.arduino.cc/projecthub/dEskoG/rduinoscope-4d399c

https://github.com/ForthWin/Forth2020UsersGroup/blob/master/ASTROLAB.F?fbclid=IwAR1WDOtVrYgl-IU7aDVg-y2659sf4HU1BfC0qWDlf0LNyo6Y_k4kRzJZ4yA


## Iterate, new hypotheses or predictions
Redo using a small moden PLC with forth 

