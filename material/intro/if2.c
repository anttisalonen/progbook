#include <stdio.h>

int main(void)
{
    for(int i = 0; i < 10; i++) {
        if(i <= 5) {
            printf("i is %d\n", i);
        } else {
            printf("i is larger than 5\n");
        }
    }
}
