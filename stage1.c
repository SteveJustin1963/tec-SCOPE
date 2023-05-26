#include <stdio.h>
#include <unistd.h>

const int pulse_data[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000};
const int angle_data[] = {80, 87, 94, 101, 108, 115, 122, 126, 128, 130};
const int data_size = sizeof(pulse_data) / sizeof(pulse_data[0]);

int main() {
    int exit_program = 0;
    while (!exit_program) {
        int pulse_rate;
        printf("Enter pulse rate in Hz (between 200 and 2000) or -1 to exit: ");
        if (scanf("%d", &pulse_rate) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (pulse_rate == -1) {
            exit_program = 1;
            continue;
        }
        if (pulse_rate < 200 || pulse_rate > 2000) {
            printf("Invalid input pulse rate\n");
            continue;
        }

        int i;
        for (i = 0; i < data_size; i++) {
            if (pulse_rate <= pulse_data[i]) {
                int diff_pulse = pulse_data[i] - pulse_data[i-1];
                int diff_angle = angle_data[i] - angle_data[i-1];
                int slope = diff_angle * 1000 / diff_pulse;  // slope multiplied by 1000 to maintain precision
                int interpolated_angle = angle_data[i-1] + slope * (pulse_rate - pulse_data[i-1]) / 1000;  // divided by 1000 to bring back to original scale
                
                printf("Interpolated angle: %d\n", interpolated_angle);
                break;
            }
        }

        if (i == data_size) {
            printf("Pulse rate is too high for the available data\n");
        }

        sleep(3);
    }

    return 0;
}

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

sudo code

DECLARE pulse_rate
DECLARE exit_program
DECLARE pulse_data[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000}
DECLARE angle_data[] = {80, 87, 94, 101, 108, 115, 122, 129, 130}
DECLARE data_size = size of pulse_data

FUNCTION main()
    SET exit_program to 0

    WHILE exit_program is not 1
        DISPLAY "Enter pulse rate in Hz (between 200 and 2000) or -1 to exit: "
        READ pulse_rate

        IF pulse_rate is -1 THEN
            SET exit_program to 1
        ELSE IF pulse_rate < 200 OR pulse_rate > 2000 THEN
            DISPLAY "Invalid input pulse rate"
        ELSE
            FOR i = 0 TO data_size - 1
                IF pulse_rate <= pulse_data[i] THEN
                    SET diff_pulse to pulse_data[i] - pulse_data[i - 1]
                    SET diff_angle to angle_data[i] - angle_data[i - 1]
                    SET slope to diff_angle * 1000 / diff_pulse
                    SET interpolated_angle to angle_data[i - 1] + slope * (pulse_rate - pulse_data[i - 1]) / 1000

                    DISPLAY "Interpolated angle: " + interpolated_angle
                    BREAK
                END IF
            END FOR

            IF i is equal to data_size THEN
                DISPLAY "Pulse rate is too high for the available data"
            END IF
        END IF

        CALL sleep(3)
    END WHILE

    RETURN 0
END FUNCTION

CALL main()

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

 In Forth, you need to remember that it's a stack-based language, so calculations are in reverse Polish notation. 
 You also need to manually manage the control flow of your loops and conditions, often using the stack to pass values around. 
    The "begin" and "again" words form a loop, and "if" "then" and "else" handle conditionals. For array access, we're using the "@" operator, 
    which fetches the value at a given memory location (given by adding an offset to the array's base address).

Please note that this is a very approximate conversion and there might be some discrepancies, especially if your Forth system doesn't support 
all of the used features. Forth can be very different between systems, and it's also not designed to be used in quite the same way as languages like the one you've provided.

variable pulse_rate
variable exit_program
2 variable diff_pulse
2 variable diff_angle
2 variable slope
2 variable interpolated_angle

10 constant data_size
decimal 200 400 600 800 1000 1200 1400 1600 1800 2000 data_size allot
decimal 80 87 94 101 108 115 122 129 130 data_size allot

: main
    0 exit_program !
    begin
        exit_program @ 1 = if
            exit
        then
        cr ." Enter pulse rate in Hz (between 200 and 2000) or -1 to exit: " 
        pulse_rate @ 
        if pulse_rate @ -1 = 
        then 
            1 exit_program !
        else pulse_rate @ 200 < pulse_rate @ 2000 > or
            if
                cr ." Invalid input pulse rate"
            else
                0 data_size 1- do
                    pulse_rate @ i cells + @ <= if
                        i cells + @ i 1- cells + @ - 2 diff_pulse !
                        i cells + @ i 1- cells + @ - 2 diff_angle !
                        diff_angle @ 1000 * diff_pulse @ / 2 slope !
                        slope @ pulse_rate @ i 1- cells + @ - * 1000 / i 1- cells + @ + 2 interpolated_angle !
                        cr ." Interpolated angle: " interpolated_angle @ .
                        unloop exit
                    then
                loop
                data_size @ i = if
                    cr ." Pulse rate is too high for the available data"
                then
            then
        then
        3 sleep
    again ;

main

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

another

73 variable pulse_rate
variable exit_program
2 variable diff_pulse
2 variable diff_angle
2 variable slope
2 variable interpolated_angle

10 constant data_size
decimal 200 400 600 800 1000 1200 1400 1600 1800 2000 data_size allot
decimal 80 87 94 101 108 115 122 129 130 data_size allot

: main
    0 exit_program !
    begin
        exit_program @ 1 = if
            exit
        then
        cr ." Enter pulse rate in Hz (between 200 and 2000) or -1 to exit: " 
        pulse_rate @ 
        if pulse_rate @ -1 = 
        then 
            1 exit_program !
        else pulse_rate @ 200 < pulse_rate @ 2000 > or
            if
                cr ." Invalid input pulse rate"
            else
                0 data_size 1- do
                    pulse_rate @ i cells + @ <= if
                        i cells + @ i 1- cells + @ - 2 diff_pulse !
                        i cells + @ i 1- cells + @ - 2 diff_angle !
                        diff_angle @ 1000 * diff_pulse @ / 2 slope !
                        slope @ pulse_rate @ i 1- cells + @ - * 1000 / i 1- cells + @ + 2 interpolated_angle !
                        cr ." Interpolated angle: " interpolated_angle @ .
                        unloop exit
                    then
                loop
                data_size @ i = if
                    cr ." Pulse rate is too high for the available data"
                then
            then
        then
        3 sleep
    again ;

main

\\\\\\\\\\\\\\\\\\\\\\\\\
                                                        
