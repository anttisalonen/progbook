#include <stdio.h>

void increment_by_one(int *p1, int *p2)
{
        *p1 = *p1 + 1;
        *p2 = *p2 + 1;
}

int main(void)
{
        int a = 42;
        int b = 5;
        increment_by_one(&a, &b);
        printf("The value of a is: %d\n", a);
        printf("The value of b is: %d\n", b);
}
