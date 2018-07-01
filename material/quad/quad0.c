#include <stdio.h>
#include <math.h>

int main(void)
{
    float a = 3.0f;
    float b = -2.0f;
    float c = -3.0f;
    float r1, r2;
    float det = b * b - 4.0f * a * c;
    int num_roots;
    if(det < 0.0f)
        num_roots = 0;
    r1 = (-b + sqrt(det)) / (2.0f * a);
    if(det == 0.0f)
        num_roots = 1;
    r2 = (-b - sqrt(det)) / (2.0f * a);
    num_roots = 2;
    printf("Number of roots: %d\n", num_roots);
    if(num_roots == 1)
        printf("Root: %.2f\n", r1);
    else if(num_roots == 2)
        printf("Root 1: %.2f - Root 2: %.2f\n", r1, r2);
}
