#include <stdio.h>

void double_input(int *i)
{
	*i = *i * 2;
}

int main(void)
{
	int a = 3;
	double_input(&a);
	printf("%d\n", a);
}
