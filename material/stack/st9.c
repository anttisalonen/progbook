#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
	int *a = (int *)malloc(20 * sizeof(int));
	assert(a);
	memset(a, 0x00, 20 * sizeof(int));
	a[0] = 3;
	*(a + 1) = 4;
	int *b = a;
	b += 2;
	*b = 5;
	printf("%d\n", a[2]);
	free(a);
}

