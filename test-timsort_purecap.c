#include "lib/timsort_lib_purecap.h"
#include <assert.h>
#include <cheriintrin.h>


bool arrEq(int arr_a[], int arr_b[])
{

	assert(cheri_length_get(arr_a) == cheri_length_get(arr_b));
	for (size_t ix = cheri_base_get(arr_a); ix <= cheri_length_get(arr_a); ix++)
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

	size_t mid_a = 4; 	
	size_t mid_b = 5; 

	int* arr_a_base_set = cheri_offset_set(input_arr_mutate_a_input, mid_a);
	int* arr_a_base_length_set = cheri_bounds_set(arr_a_base_set, cheri_length_get(input_arr_mutate_a_input) - mid_a );

	int* arr_b_base_set = cheri_offset_set(input_arr_mutate_b_input, mid_b);
	int* arr_b_base_length_set = cheri_bounds_set(arr_b_base_set, cheri_length_get(input_arr_mutate_b_input) - mid_b );

	merge(input_arr_mutate_a_input, arr_a_base_length_set);
	merge(input_arr_mutate_b_input, arr_b_base_length_set);

	assert(arrEq(input_arr_mutate_a_input, input_arr_mutate_a_expected));
	assert(arrEq(input_arr_mutate_b_input, input_arr_mutate_b_expected));

	return;
}

void test_isSorted()
{
	// positive cases
	int sorted_array_empty[] = {};
	int sorted_array_singleton[] = {42};

	assert(isSorted(sorted_array_empty));
	assert(isSorted(sorted_array_singleton));

	int sorted_array_small[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	assert(isSorted(sorted_array_small));

	// negative cases
	sorted_array_small[5] = 42;
	assert(!isSorted(sorted_array_small));
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
	assert(!isSorted(arr));

	// sort the data
	timSort(arr);

	// check that have done real work
	assert(isSorted(arr));

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
