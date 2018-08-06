#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* filt: filter out positive numbers from an int array 
 * Input parameters:
 *   arr: pointer to array with numbers
 *   len: number of elements in arr
 *
 * Output parameters:
 *   return value: pointer to a newly allocated array with only
 *                 negative numbers
 *   end_len: number of elements in return value
 *
 * Return value may be NULL if no memory available.
 * Otherwise, caller must free() the return value after use.
 */
int *filt(int *arr, int len, int *end_len)
{
	// first allocate memory large enough to hold the
	// whole array in case this is needed
	int *res = (int *)malloc(sizeof(int) * len);
	if(!res) {
		perror("malloc");
		return NULL;
	}
	memset(res, 0x00, sizeof(int) * len);

	// end_len serves as our index so we know where to
	// put the next number
	*end_len = 0;

	for(int i = 0; i < len; i++) {
		if(*arr < 0) {
			res[*end_len] = *arr;
			(*end_len)++;
		}
		arr++;
	}

	// resize the size of the output buffer
	int *new_res = (int *)realloc(res, sizeof(int) * *end_len);
	if(!new_res) {
		perror("realloc");
		free(res);
		return NULL;
	}
	return new_res;
}

int main(void)
{
	int inp[8] = {3, -1, 4, -2, 5, -3, 6, -4};
	int end_len;
	int *res = filt(inp, 8, &end_len);
	if(!res) {
		return 1;
	}
	for(int i = 0; i < end_len; i++) {
		printf("%d\n", res[i]);
	}
	free(res);
	return 0;
}
