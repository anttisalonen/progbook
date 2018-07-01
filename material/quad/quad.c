#include <stdio.h>
#include <math.h>

int quad(float a, float b, float c, float *r1, float *r2)
{
    float det = b * b - 4.0f * a * c;
    if(det < 0.0f)
        return 0;
    *r1 = (-b + sqrt(det)) / (2.0f * a);
    if(det == 0.0f)
        return 1;
    *r2 = (-b - sqrt(det)) / (2.0f * a);
    return 2;
}

int main(void)
{
    float a = 3.0f;
    float b = -2.0f;
    float c = -3.0f;
    float r1, r2;
    int num_roots = quad(a, b, c, &r1, &r2);
    printf("Number of roots: %d\n", num_roots);
    if(num_roots == 1)
        printf("Root: %.2f\n", r1);
    else if(num_roots == 2)
        printf("Root 1: %.2f - Root 2: %.2f\n", r1, r2);
}
