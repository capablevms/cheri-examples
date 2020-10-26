#include "include/timsort_lib.h"

/**
 * Sorts the input array, in place, using `insertion sort`.
 * @param arr array to sort
 */
void insertionSort(bp_array bp)
{
	size_t base = get_base(bp);
	size_t length = (get_length(bp) / sizeof(int)) - base;

	int *arr = get_pointer(bp);
	inspect_pointer(arr);

	int ix, ixValue, ixP;
	for (ix = base + 1; ix < (base + length); ix++)
	{
		ixValue = arr[ix];
		ixP = ix - 1;

		while (ixP >= 0 && arr[ixP] > ixValue)
		{
			arr[ixP + 1] = arr[ixP];
			ixP = ixP - 1;
		}
		arr[ixP + 1] = ixValue;
	}
}

/**
 * Merges two runs of an array.
 * @param arr super-array to merge
 */
void merge(bp_array bp)
{
	printf("merge \n");

	// allocations
	size_t lengthFirstHalf = get_base(bp);
	size_t lengthSecondHalf = (get_length(bp) / sizeof(int)) - lengthFirstHalf;

	int *arr = get_pointer(bp);

	int firstHalf[lengthFirstHalf];
	int secondHalf[lengthSecondHalf];

	// copy to intermediate storage
	memcpy(&firstHalf[0], &arr[0], lengthFirstHalf * sizeof(int));
	memcpy(secondHalf, &arr[lengthFirstHalf], lengthSecondHalf * sizeof(int));

	// merge intermediate back to output
	size_t ix_fst = 0;
	size_t ix_snd = 0;
	size_t ix_out = 0;
	while ((ix_fst < lengthFirstHalf) && (ix_snd < lengthSecondHalf))
	{
		if (firstHalf[ix_fst] <= secondHalf[ix_snd])
		{
			arr[ix_out++] = firstHalf[ix_fst++];
		}
		else
		{
			arr[ix_out++] = secondHalf[ix_snd++];
		}
	}

	// copy stragglers
	if (ix_fst < lengthFirstHalf)
	{
		size_t delta = lengthFirstHalf - ix_fst;
		memcpy(&arr[ix_out], &firstHalf[ix_fst], delta * sizeof(int));
		ix_out += delta;
		ix_fst += delta;
	}

	if (ix_snd < lengthSecondHalf)
	{
		size_t delta = lengthSecondHalf - ix_snd;
		memcpy(&arr[ix_out], &secondHalf[ix_snd], delta * sizeof(int));
		ix_out += delta;
		ix_snd += delta;
	}

	return;
}

/**
 * Timsort routine for an array of `int`.
 * @param arr Array to sort
 */
void timSort(bp_array arr)
{
	size_t length = get_length(arr) / sizeof(int);

	if (length <= 1)
	{
		return;
	}

	// insertion sort on `RUN_LENGTH` segments
	for (size_t ix = 0; ix < length; ix += RUN_LENGTH)
	{
		size_t max_ix = min((ix + RUN_LENGTH) + 1, length);
		callBP(insertionSort, get_pointer(arr), ix, max_ix * sizeof(int));
	}
	// Merge window doubles every iteration
	for (size_t size = RUN_LENGTH; size < length; size *= 2)
	{
		// Merge
		for (size_t left = 0; left + size < length - 1; left += 2 * size)
		{
			size_t mid = left + size;
			size_t right = min((left + 2 * size), length);

			callBP(merge, &(get_pointer(arr)[left]), mid, right * sizeof(int));
		}
	}
}

/**
 * Allocates a large chunk of memory, copies data to it and then sorts using
 * `timsort`. The example uses two implementations, one that uses explicit parameters to encode run
 * lengths and another that uses capabilities.
 * @param arr array to print
 * @return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[])
{
	for (size_t sz = 1; sz <= MAX_ARRAY_SZ; sz++)
	{
		// place data to be sorted on the heap and make a copy
		int *arr = random_chunk(sz);
		int *arr_cpy = malloc(sz * sizeof(int));

		assert(NULL != arr);
		assert(NULL != arr_cpy);

		memcpy(arr_cpy, arr, sz * sizeof(int));

		// check copy
		assert(arrEq(arr, arr_cpy, 0, sz - 1));

		// sort initial data set with TimSort
		callBP(timSort, arr, 0, sz * sizeof(int));

		// sort the copy with a stable quicksort
		qsort(arr_cpy, sz, sizeof(int), cmpfunc);

		// check we have done real work
		assert(isSorted(arr, sz));

		// check the sorting is correct
		assert(arrEq(arr, arr_cpy, 0, sz - 1));

		// clean up
		free(arr);
		free(arr_cpy);
	}

	return EXIT_SUCCESS;
}