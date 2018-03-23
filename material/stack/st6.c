#include <stdio.h>
#include <string.h>

int main(void)
{
	int my_array[5];
	memset(my_array, 0x00, sizeof(my_array));
	my_array[0] = 3;
	my_array[1] = 2;
	printf("%d\n", my_array[1]);
}

