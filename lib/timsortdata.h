#include <cheriintrin.h>
#include <stdlib.h>
#include <time.h>

int *random_chunk(size_t arr_length)
{

	srand(time(NULL));

	int *arr = malloc(arr_length * sizeof(int));
	for (size_t ix = 0; ix < arr_length; ix++)
	{
		arr[ix] = rand();
	}

	return arr;
}