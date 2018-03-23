#include <stdio.h>
#include <string.h>

void func(int *array, int len)
{
	for(int i = 0; i < len; i++) {
		*array = *array * 2;
		array++;
	}
}

int main(void)
{
	int my_array[5];
	memset(my_array, 0x00, sizeof(my_array));
	my_array[0] = 3;
	my_array[1] = 2;
	func(my_array, 5);
	printf("%d\n", my_array[1]);
}

