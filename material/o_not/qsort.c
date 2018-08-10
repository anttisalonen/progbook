#include <stdio.h>
#include <stdlib.h>

int comparison(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

int main(void)
{
    int my_array[5] = { 3, 7, 5, 1, 8 };
    qsort(my_array, 5, sizeof(int), comparison);
    for(int i = 0; i < 5; i++) {
        printf("%d\n", my_array[i]);
    }
}
