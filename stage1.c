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

