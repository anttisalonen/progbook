#include <stdio.h>

int double_input(int i)
{
	return i * 2;
}

int main(void)
{
	int a = 3;
	int b;
	b = double_input(a);
	printf("%d\n", b);
}
