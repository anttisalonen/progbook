#include <stdio.h>

int square(int x)
{
    return x * x;
}

int main(void)
{
    for(int i = 0; i < 10; i++) {
        int i_squared = square(i);
        if(i_squared > 10) {
            printf("%d\n", i_squared);
        }
    }
}
