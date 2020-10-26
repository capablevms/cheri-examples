#include "lib/timsort_lib.h"
#include <assert.h>

bool arrEq(int arr_a[], int arr_b[], size_t lowerBound, size_t upperBound)
{
	for (size_t ix = lowerBound; ix <= upperBound; ix++)
	{
		if (arr_a[ix] != arr_b[ix])
		{
			return false;
		}
	}

	return true;
}

void test_merge()
{
	int input_arr_control[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	int input_arr_mutate_a_input[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int input_arr_mutate_a_expected[] = {6, 5, 4, 3, 2, 1, 10, 9, 8, 7};

	int input_arr_mutate_b_input[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int input_arr_mutate_b_expected[] = {5, 4, 3, 2, 1, 10, 9, 8, 7, 6};

	merge(input_arr_mutate_a_input, 0, 4, 10);
	merge(input_arr_mutate_b_input, 0, 5, 10);

	assert(arrEq(input_arr_mutate_a_input, input_arr_mutate_a_expected, 0, 9));
	assert(arrEq(input_arr_mutate_b_input, input_arr_mutate_b_expected, 0, 9));

	return;
}

void test_isSorted()
{
	// positive cases
	int sorted_array_empty[] = {};
	int sorted_array_singleton[] = {42};

	assert(isSorted(sorted_array_empty, 0));
	assert(isSorted(sorted_array_singleton, 1));

	int sorted_array_small[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	assert(isSorted(sorted_array_small, 10));

	// negative cases
	sorted_array_small[5] = 42;
	assert(!isSorted(sorted_array_small, 10));
}

void test_timsort()
{
	int data[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	const size_t arr_length = 20;

	// place the chunk of data on the heap
	int *arr = malloc(arr_length * sizeof(int));

	assert(NULL != arr);

	for (size_t ix = 0; ix < arr_length; ix++)
	{
		arr[ix] = data[ix];
	}

	// ensure we start fair ( the data is not initially sorted )
	assert(!isSorted(arr, arr_length));

	// sort the data
	timSort(arr, arr_length);

	// check that have done real work
	assert(isSorted(arr, arr_length));

	// clean up
	free(arr);
}
/**
 * Test harness for `timsort.c`.
 * @return EXIT_SUCCESS when all tests pass. EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[])
{
	test_isSorted();

	test_merge();

	test_timsort();

	return EXIT_SUCCESS;
}
