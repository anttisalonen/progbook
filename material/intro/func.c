#include <stdio.h>

void my_func(int x)
{
    printf("Parameter: %d\n", x);
}

int main(void)
{
    my_func(5);
    int x = 7;
    my_func(x);
}
