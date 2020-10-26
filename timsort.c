#include "lib/timsort_lib.h"
#include "lib/timsortdata.h"
#include <cheriintrin.h>

/**
 * Allocates a large chunk of memory, copies a large chunk of data to it and then sorts using
 * `timsort`. The example uses two implementations, one that uses explicit parameters to encode run
 * lengths and another that uses capabilities.
 * @param arr array to print
 * @return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[])
{

	int *arr = random_chunk();

	// place the chunk of data on the heap
	if (NULL == arr)
	{
		return EXIT_FAILURE;
	}

	// sort the data
	timSort(arr, cheri_length_get(arr) / sizeof(int));

	// clean up
	free(arr);

	return EXIT_SUCCESS;
}