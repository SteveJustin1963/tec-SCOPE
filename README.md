

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)

### forward

Az Istenek áldásával and in dedication to my late father, uram Jusztin Istvan (Stephen Justin) Born 1932- RIP 2000, from Kiskunfélegyháza Magyarország Hungary, who later emigrated to Australia in 1962. His passage into astronomy was in the late 1940s, he hand made an 8 inch reflector telescope with his friends. Much later in 1976 when I was 13 years old, I saw a repeating advert in the TV week "Make your own telescope! by Astro Optical at Clarke Lane, Crows Nest, Sydney, NSW, Australia". My dad took interest and 3 years later we finished building a 12.5 inch Fény-Vödör reflector telescope. Id like to acknowledge Kiskunfélegyházi Csillagászati Egyesület; the local astronomy club in Kiskunfélegyháza. Now 42 years later I hope to control this old telescope with a TEC-1, MINT and Stellarium star maps.    


### project
that involves using rotary encoders to read and control a telescope. I wrote some code to convert the encoder data into map data, which I then sent to Stellerium.   



1.2.2019 chat Peter Forth re planetarium s/w https://github.com/PeterForth/Planetarium and nodemcu esp8266. Win32forth group, members Astronomers; Michel has an astronomy project on ForthWin also. Iruata souza is astrophysicist . Roland Herrera is also hobby astronomer, so we have lots of people interested, in astronomy graphs. 
Michel Jean the professor from Canada started an own Stellarium on ForthWin. I have Arduino Mega2560, I use them to controll a Mearm Robot and other things, I think it can be easy to control a stepper motor drive for 2axis for the telescope mount.

1.3.2022 chat Craig Jones , OK, so you have serial streams of x and y data coming into the TEC? I wouldn't use another bitbang, I'd use 2 x 6850's with interrupts. I have done the circular buffered serial interrupt code for the 6850, no big deal to have two instances. I guess Stellerium will need a serial port too? Then is it 3 serial ports? sj; the x and are not serial streams sorry, they a two pairs of phase shifted dc pulses. 600 pulse pairs per revolution. this will be geared to a larger disk on the shaft of the telescope. as the telescope is moved the out of phase pulse pair from x is fed to 2 ports and y pulse pair is fed to 
the next 2 ports. this info is maths converted in mint to astronomy conordiants then into ascii codes that are up loaded via a second com port to a laptop running a serial plug in for free stellerium which then moves the cross hairs on the star map. two 6860 sounds better. i know reading the 4 data ports will be very busy task as slight movement result in huge pluse trains. when its converted the final ascii, the update is tiny. i have a 12 inch reflector telescope at home and access to smaller reflectors, no refactors. the small ones are really cheap to pickup. its easy enough to point and look but one never knows what there looking at.  we'll know with mint we can later add stepper or dc motors, track objects, add a joy stick, more addons lol ques re the mint code ? re this symbol ? reads serial port, so with a second bit bang and say mod the ? into   ?< and ?> commands cj; It was always the idea with MINT (well my idea anyway) to add buffered serial interrupts to it, the first pass was to get it going in 2k for the basic TEC-1. I did the buffered serial interrupt code a while ago but have yet to incorporate it into MINT, now I will have to consider more than one serial port as well! I will leave the implementation in MINT to John. Obviously MINT will need to become bigger if it has to handle APU and serial buffered comms!

23.3.2022 ordered 2 rotary encoders, 600p/r

17.9.22 - recycled broken foot massage machine for 24v dc worm drive. now need another. or go stepper motor way. try both. 

22.9.22 - investigate counter with flipflop cct to count rotary encoder movement without resorting to interrupts to count. investigate dc vs steppers. checked LX200 ascii codes. checked io port options.
