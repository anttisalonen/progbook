#include <stdio.h>

int square(int x)
{
    return x * x;
}

int main(void)
{
    int x = 5;
    int x_squared = square(x);
    printf("%d squared is %d\n", x, x_squared);
}
