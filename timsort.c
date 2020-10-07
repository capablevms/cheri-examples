#include "include/common.h"
#include <cheri/cheric.h>
#include <cheriintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int RUN = 4;

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
		bool hitBottom=false; 

		while ((ixp >= lowerBound) && (arr[ixp] > ix_value))
		{
			arr[ixp + 1] = arr[ixp];

			if (ixp > 0)
			{
				ixp--;
			} else {
				arr[ixp] = ix_value;
				hitBottom=true;
				break;
			}
		}

		if(!hitBottom) {
			arr[ixp+1] = ix_value;
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
int merge(int arr[], size_t lowerBound, size_t midPoint, size_t upperBound)
{
	// sanity check
	if (lowerBound > midPoint || midPoint > upperBound)
	{

		return EXIT_FAILURE;
	}

	// allocations
	size_t lengthFirstHalf = midPoint - lowerBound;
	size_t lengthSecondHalf = upperBound - midPoint;

	int firstHalf[lengthFirstHalf];
	int secondHalf[lengthSecondHalf];

	// copy to inntermediate storage
	memcpy(firstHalf, &arr[lowerBound], lengthFirstHalf);
	memcpy(secondHalf, &arr[midPoint + 1], lengthSecondHalf);

	// merge intermediate back to output
	size_t ix_fst, ix_snd, ix_out = 0;
	while ((ix_fst < lengthFirstHalf) && (ix_snd < lengthSecondHalf))
	{
		arr[ix_out++] =
			(firstHalf[ix_fst] <= secondHalf[ix_snd]) ? firstHalf[ix_fst++] : secondHalf[ix_snd++];
	}

	// copy straglers
	if (ix_fst < lengthFirstHalf)
	{
		memcpy(&arr[ix_out], &firstHalf[ix_fst], lengthFirstHalf - ix_fst);
		ix_fst += lengthFirstHalf - ix_fst;
	}
	if (ix_snd < lengthSecondHalf)
	{
		memcpy(&arr[ix_out], &secondHalf[ix_snd], lengthSecondHalf - ix_snd);
		ix_snd += lengthSecondHalf - ix_snd;
	}

	return EXIT_SUCCESS;
}

/**
 * Prints out an integer array. Uses capability instructions to determine the array's length
 * @param arr array to print
 * @return EXIT_SUCCESS on success. EXIT_FAILURE otherwise
 */
uint cap_printArray(int arr[])
{
	if cheri_is_invalid (arr)
	{
		printf(
			"Supplied argument is not a valid capability and we can't determine the safe length");
		return EXIT_FAILURE;
	}

	size_t length = cheri_length_get(arr) / sizeof(int);

	printf("Length: %lu  \n", length);

	for (size_t ix = 0; ix < length; ix++)
	{
		printf("%d  ", arr[ix]);
	}

	printf("\n");
	return EXIT_SUCCESS;
}

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

void timSort(int arr[], int n)
{
	
	for (size_t i = 0; i < n; i += RUN)
	{
		insertionSort(arr, i, min((i + 31), (n - 1)));
	}
		
	for (size_t size = RUN; size < n; size = 2 * size)
	{

		for (size_t left = 0; left < n; left += 2 * size)
		{

			size_t mid = left + size - 1;
			size_t right = min((left + 2 * size - 1), (n - 1));

			merge(arr, left, mid, right);
		}
	}
}

int main(int argc, char *argv[])
{

	int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	printf("Initial array: ");
	cap_printArray(arr);

	timSort(arr, 20);

	printf("Sorted array: ");
	cap_printArray(arr);
	printf("\n");

	return EXIT_SUCCESS;
}
