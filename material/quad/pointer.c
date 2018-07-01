#include <stdio.h>

int main(void)
{
        int a = 42;
        int *p;
        p = &a;
        *p = 45;
        printf("The value of a is: %d\n", a);
        printf("The value of p is: %p\n", p);
        printf("The value of what p points to is: %d\n", *p);
}
