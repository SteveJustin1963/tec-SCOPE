make sure you join a club if you use a laser pointer >1mW
- https://www.police.nsw.gov.au/__data/assets/pdf_file/0008/139895/List_of_Astronomical_Societies.pdf


work in progress, not finshed and tested


## Let your Tec-1D control your 8-inch F5 Dobson telescope

This style of mirror based telescope is pushed by hand. We can leap to a new level of control using motors and encoders coupled to free planetarium software like Stellarium via a simple serial link.

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)

 

### mount
- alt and azimuth (x and y) are friction coupled, convert to gears and bearing support 
- we need 2 motors and 2 encoders
- the dobson needs to be modified so movement is made on shafts and gears
- reduction gears; use foot massage motor with its gear drive and double shaft
![image](https://github.com/user-attachments/assets/f66e2597-9af3-4984-bb4c-550e19ec3681)



### encoders
  - HN3806-AB, AB 2 phase rectangular orthogonal pulses 600 pulses/rev, max 5000rpm, response 0-20KHz 
  - green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
  - should not connect AB directly VCC 
  - feed into quadrature clock converter LS7183 then into two 74193-up-down-counters (4bit)
 ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)



### control
- we will ise a simple  x y joystick slew it around with the dc motors
- and also via via tec1 commands
- or track a position using an optical gates or estimation 
- will try starmap Stellarium interface via another bitbang port x.200 commands
- what to do for long-exposure astrophotography ?

 
### code 
https://github.com/SteveJustin1963/tec-SCOPE/tree/master/code

work to be done 
- read encoders x2 with interrupt start
- motor control x2
- display data
  - read data from encoders, convert to angle, display on tec1s seven segment display of 6 digits xxxx xx
  - press A gives the azimuth and alt together to 1 degree accuracy, eg az= 0299 alt = 23 displayed as 0299 23
  - press B then only az is displayed deg-min eg 0299 44 meaning 299 degs 44 minutes
  - press C then only alt is displayed deg-min eg 0023 51 meaning 23 deg 51 min
  - no need for precision in seconds unless the machanicals of the telescope can justify it  
- Stellarium integration
  - /www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/

- calibration
  - angle check using iphone for compass,
    - it has a level guide in it as well with the little cross in the middle. adjust thumb screw in base
    - true north in Sydney add 12.83°E in 2020 changes 0.04° annually
  - a plastic protractor and plumb for angle
  - once positioned right then calibrate with alignment to a known celestial object





