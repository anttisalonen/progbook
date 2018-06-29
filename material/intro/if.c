#include <stdio.h>

int main(void)
{
    int x = 42;
    if(x < 20) {
        printf("x is smaller than 20\n");
    } else if(x < 40) {
        printf("x is smaller than 40\n");
    } else {
        printf("x is larger than or equal to 40\n");
    }
}
