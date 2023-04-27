# A TEC-1 Controlled Telescope

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/scope-steps1.png)  






In this project, we will be developing a system to control a DIY Dobson 200mm F5 telescope in three stages. Here's my $35 Gumtree find to play with.

![image](https://user-images.githubusercontent.com/58069246/210939819-5d845b3c-116c-424e-b404-cfd4097ebc1e.png)


## In the first stage, 
We will keep the Dobson's 2 axis mount with manual movement on the altitude (vertical) and azimuth (horizontal) axes. 

Two resistor pots will be attached on each axis pivot point to measure approx position. A 555 + circuit will convert this into a variable frequency of square pulses, which will be sent to a io port on the tec1 to be counted per second. With the pulse frequency in the low and sub khz counting the pulses through the io should be ok. Using a scaling factor we interpolate to scopes angle from the freq. Additionally, we can compare the counts to previous intervals to determine the direction of movement. We can then converted this to serial data ASCII codes that Stellariumcan use to compute the position of the crosshairs on the map. After calibration with a reference star, we can confirm our observations.

![image](https://user-images.githubusercontent.com/58069246/210936069-624b8c93-c571-4490-845a-cee685932f91.png)

 

circuit simulator https://tinyurl.com/2p8fdmws

make the 220 value the pot

![image](https://user-images.githubusercontent.com/58069246/220818068-d6cbff32-57e3-4e6c-9fc0-8369b4c0e593.png)


## code
test in https://www.programiz.com/c-programming/online-compiler/
```
#include <stdio.h>
#include <unistd.h> // for sleep() function

// Define data points for linear interpolation
const int pulse_data[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000};
const int angle_data[] = {80, 87, 94, 101, 108, 115, 122, 126, 128, 130};
const int data_size = sizeof(pulse_data) / sizeof(int);

int main()
{
    while (1) // loop forever
    {
        // Read input pulse rate in the range of 200 Hz to 2 kHz
        int pulse_rate;
        printf("Enter pulse rate in Hz (between 200 and 2000): ");
        scanf("%d", &pulse_rate);
        if (pulse_rate < 200 || pulse_rate > 2000) {
            printf("Invalid input pulse rate\n");
            continue; // go back to the start of the loop
        }

        // Perform linear interpolation
        int i;
        for (i = 1; i < data_size; i++)
        {
            if (pulse_rate <= pulse_data[i])
            {
                float slope = (angle_data[i] - angle_data[i-1]) / (float)(pulse_data[i] - pulse_data[i-1]);
                float interpolated_angle = angle_data[i-1] + slope * (pulse_rate - pulse_data[i-1]);

                // Map interpolated angle to desired range
                float angle = (interpolated_angle - angle_data[0]) / (float)(angle_data[data_size-1] - angle_data[0]) * (130 - 80) + 80;

                // Convert angle to degrees, minutes, and seconds
                int degrees = (int) angle;
                float remaining_angle = (angle - degrees) * 60.0;
                int minutes = (int) remaining_angle;
                float seconds = (remaining_angle - minutes) * 60.0;

                // Print result
                printf("Angle: %d degrees %d minutes %f seconds\n", degrees, minutes, seconds);
                break;
            }
        }

        sleep(3); // wait for 3 seconds before going back to the start of the loop
    }

    return 0;
}

```
##  forth73 version of it

```
: data  { array -- } dup cells + ;
: pulse_data  200 400 600 800 1000 1200 1400 1600 1800 2000 data ;
: angle_data  80 87 94 101 108 115 122 126 128 130 data ;
: data_size  pulse_data cells / ;

: interpolated_angle { i -- f }
    pulse_data dup i cells + @ swap cells - over cells - @ - f/
    angle_data dup i cells + @ swap cells - over cells - @ - f/
    f/ f* f+ ;

: map_angle { f -- deg min sec }
    angle_data 0 cells + @ f- angle_data data_size cells 1- cells + @ angle_data 0 cells + @ f- f/
    130 80 f- f* 80 f+ f/
    fround dup f- fround swap f- 60 f* fround ;

: .angle { deg min sec -- }
    ." Angle: " dup . ." degrees " dup . ." minutes " f. ." seconds" cr ;

: main
    begin
        cr ." Enter pulse rate in Hz (between 200 and 2000): " flush
        0 0
        begin
            dup
            begin
                dup cr ." Invalid input pulse rate" until
                cr ." Enter pulse rate in Hz (between 200 and 2000): " flush
            200 < -1 > 2000 > or
            until
            >r
            data_size 1- 0 do
                dup i cells + @ over r@ <= over i 1+ cells + @ over <= and
                if
                    interpolated_angle map_angle .angle
                    drop 2drop r> exit
                then
            loop
            r> drop
        3 000 ms
        again
    drop
    0 ;
```

 
 


## In the second stage, 
we will attach two DC motor drives to the mount using threaded rods and nuts that act as a worm drive. A slotted wheel on the shaft will interrupt an optical switch to count the rotations, and this data will be sent to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. The resistor pots on the mount axes will remain in place to compare measurements. The relationship between the number of turns and the angle of the mount is not linear due to the cam, so we will use code and trigonometry functions to calculate an accurate position. The worm drive and cam radius design also improve torque control and reduce backlash. We will add switches to slew the DC motors to aim at celestial objects, and we will use the TEC-1 to control the motors to track or hold onto celestial objects.

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/shaft-cont-1.png)
![image](https://user-images.githubusercontent.com/58069246/210935138-3a75fd25-d7a7-4a21-80bd-966cc343f6ca.png)
![image](https://user-images.githubusercontent.com/58069246/210935157-8995b2e2-70dd-4a91-8b7a-2bb5ee303c58.png)






## In the third stage, 
we will remove the cams and replace the DC motors with stepper motors, which will be geared at a 6:1 ratio or higher, whith a wheel mounted to the side of the axis. This will allow us to achieve more precise positioning. We will also remove the resistor pots and replace them with rotary encoders to measure the position of the motors and send this data to the TEC-1, which will use MINT to convert it into ASCII codes and send it to Stellarium. With the gearing and software enhancements, we will be able to increase position control and implement torque compensation to maintain a constant level of force on the telescope, reducing shaking and improving stability. Slewing will be done by software rather than DC switches, allowing us to move the telescope to specific locations on the sky map and track celestial objects as they move across the sky or follow moving ones like satellites.


![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23.png)

 


## Rotary Encoder HN3806-AB 

![](https://github.com/SteveJustin1963/tec-SCOPE/blob/master/pics/3-23-2.png)

The Rotary Encoder HN3806-AB is a device that is used to measure the rotational speed, angle, acceleration, and length of an object. It generates 600 pulses per revolution and has a green A phase, white B phase, red Vcc power +, and black V0. It is powered by a DC5-24V power source and has a shaft size of 613mm. The encoder has a size of 3835.5mm and an output of AB 2phase rectangular orthogonal pulses. It has a maximum mechanical speed of 5000 R/min and a response frequency of 0-20KHz. The encoder comes with a 1.5 meter cable and should not have its AB 2phase output directly connected to VCC to avoid damaging the output triode. It is suitable for use in intelligent control and various displacement measurement applications.

i need to add a Timing Belt Drive Pulley to its 6mm shaft. will contrstruct a wood wheel and invert a drive pully with the teeth facing out so the drive pully can interface with it

![image](https://user-images.githubusercontent.com/58069246/223073009-7ab32520-386b-4db5-81f6-1c0dc1765fc9.png)


## code examples

https://electropeak.com/learn/how-to-interface-a-rotary-encoder-with-arduino/
 

This is an Arduino sketch for reading a rotary encoder with AB 2-phase rectangular orthogonal pulse output. The rotary encoder has two output channels, A and B, which produce pulses that are 90 degrees out of phase with each other. By reading the relative phase of the A and B pulses, the sketch is able to determine the direction of rotation of the encoder.

In the setup function, the sketch sets up the serial communication at a baud rate of 9600 and configures the pin 2 and 3 as inputs with pull-up resistors. It also sets up external interrupts on pin 0 and 1, and assigns the A and B functions to be called when the interrupts are triggered.

In the loop function, the sketch checks if the value of the counter has changed since the last time it was sent over serial. If the counter has changed, it sends the new value over serial.

The A and B functions are called whenever an interrupt is triggered on pin 0 and 1, respectively. These functions check the state of the other input pin to determine the direction of rotation of the encoder. If the other input is low, the counter is incremented (in the case of A) or decremented (in the case of B). If the other input is high, the counter is decremented (in the case of A) or incremented (in the case of B).

##  read the differential and count up or down
code uses the @ operator to read the value of pins 2 and 3 before deciding whether to increment or decrement the counter variable. The @ operator retrieves the value of a memory location (in this case, the value of pins 2 and 3) and pushes it onto the data stack. The IF statement then checks the value of the top item on the data stack (either 0 or 1, depending on the value of the pin) and executes the appropriate branch.

The output of this modified code will be a series of numbers, each representing the current value of the counter variable. The value of counter will be incremented or decremented by 1 each time an interrupt is triggered on port 2 or 3, depending on the differential signal read from those pins.
 


