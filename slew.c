// slew code for  telescope

#include <stdio.h>
#include <math.h>
 
int main()
{
 
    double degree, radian, pi=3.14159265358979323846264338327950288419716939937510582097494459230781;
    printf("Please Enter the value of angle in degrees : ");
    scanf("%lf", &degree);
 
    /* 1 radian = 180/pi degree */
    radian = (degree * pi) / 180.0;
 
    printf("\n %lf Degree = %lf Radian\n\n", degree, radian);
 
    return 0;
}
