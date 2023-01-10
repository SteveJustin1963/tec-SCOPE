float interpolate_angle(int adc_val)
{
    // Lookup table with higher resolution
    int Lookup[19] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    // Angle values corresponding to the above ADC values
    float angle_lookup[19] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0};
    for (int i = 18; i > 0 ; i--) 
    {
    if (Lookup[i - 1] > adc_val) 
    {
        // Using linear interpolation
        float angle = angle_lookup[i-1] - (((float)(adc_val - Lookup[i-1]) / (float)(Lookup[i-1] - Lookup[i])) * (angle_lookup[i-1]-angle_lookup[i]));
        //Rounding off to 0.1 degree
        angle = round(angle * 10) / 10;
        return angle;
    }
    }
}
