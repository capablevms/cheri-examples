#include <stdlib.h>
#include <time.h>
#include <cheriintrin.h>

int* random_chunk()
{

	srand(time(NULL));

	// const size_t arr_length =  8192 ; 
	const size_t arr_length =  9158656 ; // borked
	// const size_t arr_length = 1008 * 756 * 3 + 1;
	int* arr = malloc(arr_length * sizeof(int));
	for (size_t ix = 0; ix < arr_length; ix++)
	{
		arr[ix] = rand();
	}

	return arr;
}