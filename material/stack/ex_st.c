#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *a = (int *)malloc(20 * sizeof(int));
	int **b = &a;
	printf("%p\n", a);
	printf("%p\n", b);
	free(a);
}

