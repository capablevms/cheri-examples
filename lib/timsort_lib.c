#include "timsort_lib.h"

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
 * Checks if an array of integers `arr` having length `length` is sorted in ascending order.
 * @param arr Array to sort
 * @param length The legth of `arr`
 */
bool isSorted(int arr[], size_t length)
{
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
 * Sorts the input array, in place, using `insertion sort`.
 * @param arr array to sort
 * @param lowerBound lower bound
 * @param upperBound upper bound
 */
void insertionSort(int arr[], size_t lowerBound, size_t upperBound)
{
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
 * Merges two sorted arrays segments into a single run.
 * @param arr array to sort
 * @param lowerBound lower bound
 * @param midPoint middle point that separates the two input runs
 * @param upperBound upper bound
 */
void merge(int arr[], size_t lowerBound, size_t midPoint, size_t upperBound)
{
	// sanity check
	assert(lowerBound <= midPoint && midPoint <= upperBound);

	// allocations
	size_t lengthFirstHalf = midPoint - lowerBound;
	size_t lengthSecondHalf = upperBound - midPoint;

	int firstHalf[lengthFirstHalf];
	int secondHalf[lengthSecondHalf];

	// copy to inntermediate storage
	memcpy(firstHalf, &arr[lowerBound], lengthFirstHalf * sizeof(int));
	memcpy(secondHalf, &arr[midPoint], lengthSecondHalf * sizeof(int));

	// merge intermediate back to output
	size_t ix_fst = 0;
	size_t ix_snd = 0;
	size_t ix_out = lowerBound;
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
 * @param length The legth of `arr`
 */
void timSort(int arr[], size_t length)
{
	// insertion sort on `RUN_LENGTH` segments
	for (size_t i = 0; i < length; i += RUN_LENGTH)
	{
		insertionSort(arr, i, min((i + RUN_LENGTH), (length - 1)));
	}
	// Merge window doubles every iteration
	for (size_t size = RUN_LENGTH; size < length; size *= 2)
	{
		// Merge
		for (size_t left = 0; left + size < length; left += 2 * size)
		{
			size_t mid = left + size;
			size_t right = min((left + 2 * size), (length - 1));

			merge(arr, left, mid, right);
		}
	}
}
