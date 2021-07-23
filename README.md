![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)

I hope to control my old 1975 refelctor telescope with the TEC-1, Forth code and Stellarium's planetarium maps.    



### Parts
- TEC-1 (circa 1983, remade 2018)
- Forth code 1970, remade John Hardy's http://github.com/jhlagado/firth


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/map.png)




## Observe and Question 
"The Forth programming language was invented by Charles Moore for control of telescopes in observatories using small computers (Moore 1980). Let's try Firth of Forth by John Hardy http://github.com/jhlagado/firth on the TEC-1 to controll my telescope.

## Theory - Testable
A TEC-1 under Forth control can drive a small telescope and simultaneously interface with planetarium software such as Stellarium. 

## Prediction
"It will work but will not be easy".   

## Method 
1. run Firth on the TEC-1 https://github.com/SteveJustin1963/tec-FORTH
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

