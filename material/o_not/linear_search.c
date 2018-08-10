#include <stdio.h>

int find_in_array(const int *arr, int num_elems, int num_to_find)
{
    for(int i = 0; i < num_elems; i++) {
        if(arr[i] == num_to_find) {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    int my_array[5] = { 3, 7, 5, 1, 8 };
    int found = find_in_array(my_array, 5, 42);
    if(found) {
        printf("Found it!\n");
    } else {
        printf("Not found\n");
    }
}
