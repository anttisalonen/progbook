#include <stdio.h>

int main(void)
{
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            int added = i + j;
            printf("%d + %d = %d\n", i, j, added);
        }
    }
}
