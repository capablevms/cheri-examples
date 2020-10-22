#include "timsort_lib_purecap.h"

#include "../include/common.h"
#include <assert.h>
#include <cheri/cheric.h>
#include <cheriintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int RUN_LENGTH = 32;

/**
 * Checks if an array of integers `arr` is sorted in ascending order. Uses capability instructions
 * to determine the array's length.
 * @param arr Array to sort
 */
bool isSorted(int* arr)
{
	assert(cheri_is_valid(arr));
	size_t length = cheri_getlen(arr) / sizeof(int);

	// short-circuit: empty and singleton arrays are always sorted.
	if (length <= 1)
	{
		return true;
	}

	for (size_t ix = 1; ix < length; ix++)
	{
		if (arr[ix - 1] > arr[ix])
		{
			return false;
		}
	}

	return true;
}

/**
 * Prints out an integer array. Uses capability instructions to determine the array's length.
 * @param arr array to print
 */
void printArray(int* arr)
{
	assert(cheri_is_valid(arr));
	size_t length = cheri_getlen(arr) / sizeof(int);

	printf("Length: %lu  \n", length);

	bool lastWasNewline = false;

	for (size_t ix = 0; ix < length; ix++)
	{
		printf("%d  ", arr[ix]);
		lastWasNewline = false;

		if (0 == ix % RUN_LENGTH && 0 != ix)
		{
			printf("\n");
			lastWasNewline = true;
		}
	}

	if (!lastWasNewline)
	{
		printf("\n");
	}
}

/**
 * Sorts the input array, in place, using `insertion sort`.
 * @param arr array to sort
 */
void insertionSort(int* arr)
{
	assert(cheri_is_valid(arr));
	size_t lowerBound = cheri_getbase(arr); 
	size_t upperBound = cheri_getlen(arr) / sizeof(int);

	for (size_t ix = lowerBound + 1; ix <= upperBound; ix++)
	{
		int ix_value = arr[ix];
		size_t ixp = ix - 1;
		bool hitBottom = false;

		while ((ixp >= lowerBound) && (arr[ixp] > ix_value))
		{
			arr[ixp + 1] = arr[ixp];

			if (ixp > 0)
			{
				ixp--;
			}
			else
			{
				arr[ixp] = ix_value;
				hitBottom = true;
				break;
			}
		}

		if (!hitBottom)
		{
			arr[ixp + 1] = ix_value;
		}
	}
}

/**
 * Merges two runs of an array. 
 * @param arr super-array to merge
 * 
 */
void merge(int* arr)
{

	// allocations
	size_t lengthFirstHalf = cheri_getoffset(arr);
	size_t lengthSecondHalf = cheri_getlen(arr)  - lengthFirstHalf;
	

	int firstHalf[lengthFirstHalf];
	int secondHalf[lengthSecondHalf];

	// copy to intermediate storage (gets optimised out)
	memcpy(firstHalf, arr, lengthFirstHalf * sizeof(int));
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

	// copy straglers
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
 * Min of two size_t arguments.
 * @param a first value to choose from
 * @param b second value to choose from
 * @return The minimum value between `a` and `b`
 */
size_t min(size_t a, size_t b)
{
	if (a <= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

/**
 * Timsort routine for an array of `int`.
 * @param arr Array to sort
 * @param n The legth of `arr`
 */


void timSort(int* arr)
{
	size_t length = cheri_getlen(arr) / sizeof(int);
	inspect_pointer(arr);
	// insertion sort on `RUN_LENGTH` segments
	for (size_t ix = 0; ix < length; ix += RUN_LENGTH)
	{
		size_t min_offset = min((ix + RUN_LENGTH), (length - 1));

		int* arr_base_set = cheri_offset_set(arr, ix);
		int* arr_base_length_set = cheri_bounds_set(arr_base_set, min_offset);

		insertionSort(arr_base_length_set);
	}
	// Merge window doubles every iteration
	for (size_t size = RUN_LENGTH; size < length; size *= 2)
	{
		// Merge
		for (size_t left = 0; left + size < length; left += 2 * size)
		{
			size_t mid = left + size;
			size_t right = min((left + 2 * size), (length - 1));

			int* arr_base_length_set = cheri_bounds_set(arr, right );

			arr_base_length_set = cheri_offset_set(arr_base_length_set, mid);
			printf("%zu, %zu, %zu\n", left, mid, right);
			merge(arr_base_length_set);
		}
	}
}
