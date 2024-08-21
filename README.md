

Integrating your tec1d with your 8-inch F5 Dobson telescope

![image](https://github.com/user-attachments/assets/94e1a293-ea97-4a59-8d1b-e965b11411c8)
![image](https://github.com/user-attachments/assets/127c3469-9b72-4adf-8157-af4c271298e7)



- convert push around scope add motors and encoders
![image](https://github.com/user-attachments/assets/27509173-1c93-455f-ab52-757443135c69)


- motors  ![image](https://github.com/user-attachments/assets/a65c79ff-79d1-4115-a1bc-1dc1dbe430ba)
  - salvage 24v dc motor and shaft from foot massage machine, u see these always thrown out


- encoders ![image](https://github.com/user-attachments/assets/1062caac-b01f-4fa3-8646-d8f7d01f09d9)
  - HN3806-AB, AB 2 phase rectangular orthogonal pulses 600 pulses/rev, max 5000rpm, response 0-20KHz 
  - green A phase, white B phase, red Vcc power +, and black V0. DC5-24V power source 
  - should not connect AB directly VCC 
  - think about using LS7183


- mount
- Harmonic+Reducer
- worm drive

- 

 functions
- crude angle check using compass and protractor, convert magnetic north to true north in Sydney add 12.83°E in 2020 changes 0.04° annually 
- crude level use a bubble float. 3 large thumb screw in base, protractor and plumb for angle
- we need to calibrate the alignment so the angles are correct, we use a known celestial object
- the current mount is called a alt / azimuth , its an x and y so we need 2 motors and 2 encoders
- we need to modify the mount so we have shafts and gears of a sort
- with code or a joystick we can slew it around or goto some co ords via tec1 keypad
- or track a position using an optical gates or estimation 
- will try starmap Stellarium interface via another bitbang port x.200 commands
- what to do for long-exposure astrophotography ?

 
code 
- we need a menu of ato f and numbers to select functions

display angle
- read data from encode, convert to angle, display on tec1 seven seg display
- we have 6 digit xxxx xx
- press A then azimuth and alt to 1 degree accuracy is given ie 0299 23 meaning 299 degs 23 deg
- or press `+` then alt is displayed to higher precision ie 0299 44 meaning 299 degs 44 minutes
- press + again we more precision ie 0044 51 meaning 44 minutes and 51 seconds
- or press `-` then azimuth is displayed to higher precision ie 0023 16 meaning 23 degs 16 minutes
- press - again we more precision ie 0016 22 meaning 16 minutes and 22 seconds  


- Timing Belt Drive Pulley to its 6mm shaft.  will construct a wood wheel and invert a drive belt with the teeth facing out so the drive pulley can interface with it
- 
![image](https://github.com/user-attachments/assets/27685599-0ccf-404d-9e11-eb6efa607bb2)



- read this https://www.instructables.com/Control-Your-Telescope-Using-Stellarium-Arduino/



  


