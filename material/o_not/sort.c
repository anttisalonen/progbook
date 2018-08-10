#include <stdio.h>

void sort_array(int *arr, int num_elems)
{
    for(int j = 0; j < num_elems; j++) {
        int smallest_index = j;
        for(int i = j + 1; i < num_elems; i++) {
            if(arr[i] < arr[smallest_index]) {
                smallest_index = i;
            }
        }
        int tmp = arr[j];
        arr[j] = arr[smallest_index];
        arr[smallest_index] = tmp;
    }
}

int main(void)
{
    int my_array[5] = { 3, 7, 5, 1, 8 };
    sort_array(my_array, 5);
    for(int i = 0; i < 5; i++) {
        printf("%d\n", my_array[i]);
    }
}
