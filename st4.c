#include <stdio.h>

int main(void)
{
	int a = 3;
	int *b;
	b = &a;
	*b = 4;
	printf("%p\n", b);
	printf("%d\n", *b);
}

