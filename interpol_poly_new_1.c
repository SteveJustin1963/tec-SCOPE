 
#include <cmath>

float interpolate_angle(float adc_val)
{
    // Data points
    float x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    float y[] = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};
    int n = sizeof(x) / sizeof(x[0]);
    float a[n];
    for (int i = 0; i < n; i++)
        a[i] = y[i];
    for (int j = 1; j < n; j++)
        for (int i = 0; i < n - j; i++)
            a[i] = (a[i + 1] - a[i]) / (x[i + j] - x[i]);
    float result = a[0];
    float term = 1;
    for (int i = 0; i < n - 1; i++) {
        term = term * (adc_val - x[i]);
        result = result + a[i + 1] * term;
    }
    return result;
}
