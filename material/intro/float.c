#include <stdio.h>

int main(void)
{
    float x = 3.4f;
    float y = 4.2f;
    float z = 2.2f * x + y;
    printf("z is %f\n", z);
    printf("z with two digits after the decimal point: %.2f\n", z);
}
