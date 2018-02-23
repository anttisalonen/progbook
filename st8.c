#include <stdio.h>

int main(void)
{
	int a = 4;
	int *b;
	b = &a;
	int *c;
	c = b + 2;
	printf("%d\n", *c);
}

