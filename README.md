

# tec-SCOPE
TEC-1 controlled telescope using Forth, tec-APUS and Stellarium.
![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)

## Abstract
Forth, TEC-1, Z80, Telescope-Control


## Dedication

Az Istenek áldásával

In dedication to my late father, uram, Jusztin Istvan (Stephen Justin) 1932-2000, and his passage into astronomy back in Kiskunfélegyháza Magyarország (Hungary). He made his own telescope by hand, an 8 inch reflector with his friends. He served his country and flew the MIG-15's and also graduated in mechanical engineering and Tool-making. He emigrated to Australia in 1962 and by 1976 when I was 13 years old, after seeing a TV week advertisement "Make your own telescope! ... Astro Optical, Clarke Lane, Crows Nest, Sydney, NSW, Australia", I convinced my father for us to make a telescope. Three years later we proudly finished our 12.5 inch reflector telescope.

I also acknowledge "Kiskunfélegyházi Csillagászati Egyesület" with pride.


## Acknowledgements
https://www.facebook.com/peter.forth.583; I like very much to program in Win32forth. and also other forths too. I also like to help new programmers with questions around win32forth. Any case you need me you can contact me. Please tell me a bit about your interests in Astronomy and Forth! Your work [SJ] with astronomy and forth is fantastic! You can play with my little "planetarium" and add features if you wish. I want to make a Celestial Sphere later and project the Stars on it. And probably later steer my Telescope through wifi, with nodemcu esp8266. https://github.com/PeterForth/Planetarium Please, write a bit of your astronomy interest and Forth interest in the Win32forth group, as a presentation of you! So the other members will see your activity and interest and respond and want to be friends too... we have many Astronomers in that group! They will love to hear from you.

SJ; HI Peter, thanks for contacting me. I fell in love with Forth 2 months ago, purchased a pile of forth books, downloaded everything I could find on it. Back in 83 I built the TEC-1 Z 80 mini system project from "Talking Electronics" (supported by https://www.facebook.com/groups/623556744820045/ and also https://www.facebook.com/groups/AusVintage/). I made it my goal of porting Forth to it, but I have just started on this path. I am learning forth with online sims, have installed winforth32 to continue learning. I have considered http://www.camelforth.com/page.php?5 ( as I think it's the smallest to fit the TEC-1) then access with terminal, or make it interact with winforth. Early 2020 John hardy release a new unique version called Firth of Forth https://github.com/jhlagado/firth. Astronomy; in 1975 I built a 12.5 inch Newtonian Reflector, still have it today. It has manual controls. I hope to write an forth xyz feedback controller to drive the telescope position and also interface with https://stellarium.org/. I also hope to write many Forth SCI and EE programs to control and study more experiments.

https://www.facebook.com/n/?groups%2F2225595150855239%2F2905458052868942%2F&aref=1587938315820231&medium=email&mid=5a43869a759f5G5af49ff0005eG5a438b33d5cc7G2a8&bcode=2.1587938317.AbxpcQkDvqEfDQ-T5G8&n_m=stevejustin1963%40gmail.com&lloc=view_on_facebook_btn


## Introduction 
My love of the 1983 TEC-1 and ye old Forth code its time to make old things work again. Now with planetarium software such as Stellarium, I hope to drive and control my old telescope, to find and explore stars again in a new way.


## Observe and Question 
"The Forth programming language was invented by Charles Moore for control of telescopes in observatories using small computers (Moore 1980). Let's prove him right ...using Firth of Forth on the TEC-1 Z80 controlling a DIY telescope.

## Theory - Testable
A TEC-1 under Forth control can drive a small telescope and simultaneously interface with planetarium software such as Stellarium. 

## Prediction
1. Its will work but will not be easy.

## Method 
2. Test with small cardboard mock up, next my 12.5" Newtonian reflector, next a big DIY via club participation.
3. Parts 
* IO, https://github.com/SteveJustin1963/tec-IO
* Serial and APU, https://github.com/SteveJustin1963/tec-APUS
* Rotary encoders, addon or inside motor
* Gears, telescope shafts, and maybe in motors to increase torque
* Stepper motors, https://github.com/SteveJustin1963/tec-Stepper-Motor-Control
4. Forth control
* ASM routines
* Forth routines
* Stellarium routines
5. Additional 
* mounted camera for photo compositing
* automation 
  * timer to position 
  * tracking 
  * tcp/udp socket remote control


