#include <stdio.h>

float area(float radius)
{
    return 3.14f * radius * radius;
}

int main(void)
{
    for(int i = 1; i <= 10; i++) {
        float a = area(i);
        if(a >= 10 && a <= 100) {
            printf("%d -> area %.2f\n", i, a);
        }
    }
}
