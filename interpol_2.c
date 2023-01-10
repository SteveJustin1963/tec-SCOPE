 
 
#include <stdint.h>

uint8_t interpolate_angle(uint8_t adc_val)
{
    // Lookup table with higher resolution
    uint8_t lookup_adc[51] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
    // Angle values corresponding to the above ADC values
    uint8_t lookup_angle[51] = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130};

    for (int i = 50; i > 0; i--)
    {
        if (lookup_adc[i - 1] > adc_val)
        {
            // Using linear interpolation
            uint8_t angle = lookup_angle[i - 1] - (((float)(adc_val - lookup_adc[i - 1]) / (float)(lookup_adc[i - 1] - lookup_adc[i])) * (lookup_angle[i - 1] - lookup_angle[i]));
            return angle;
        }
    }
}
