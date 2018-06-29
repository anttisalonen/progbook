#include <stdio.h>

int main(void)
{
    int x = 5;
    if(x <= 10 && x >= 0) {
        printf("x is between 0 and 10\n");
    }
    if(x < 0 || x > 10) {
        printf("x is either negative or larger than 10\n");
    }
}
