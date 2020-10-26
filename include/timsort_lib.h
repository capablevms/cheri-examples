#ifndef __TIMSORT_LIB_H
#define __TIMSORT_LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include "common.h"
#include <cheri/cheri.h>
#include <cheri/cheric.h>
#endif

const int MAX_ARRAY_SZ = 2048;

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

int cmpfunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

bool arrEq(int arr_a[], int arr_b[], size_t lowerBound, size_t upperBound)
{
	if (lowerBound == upperBound || 0 == upperBound)
	{
		return true;
	}

	for (size_t ix = lowerBound; ix <= upperBound; ix++)
	{
		if (arr_a[ix] != arr_b[ix])
		{
			return false;
		}
	}
	return true;
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
 * Checks if an array of integers `arr` having length `length` is sorted in ascending order.
 * @param arr Array to sort
 * @param length The legth of `arr`
 */
bool isSorted(int arr[], size_t length)
{
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

struct bp_array_s
{
	void *pointer;
	size_t base;
	size_t length;
};

struct bp_array_s packBP_mangled(void *pointer, size_t baseIndex, size_t sizeInBytes)
{
	struct bp_array_s ret;

	ret.pointer = pointer;
	ret.base = baseIndex;
	ret.length = sizeInBytes;

	return ret;
}

__attribute__((always_inline)) void *get_pointer_mangled(struct bp_array_s bp)
{
	return bp.pointer;
}

__attribute__((always_inline)) size_t get_base_mangled(struct bp_array_s bp)
{
	return bp.base;
}

__attribute__((always_inline)) size_t get_length_mangled(struct bp_array_s bp)
{
	return bp.length;
}

#ifndef __CHERI_PURE_CAPABILITY__

typedef struct bp_array_s bp_array;
#define packBP(x, y, z) packBP_mangled((x), (y), (z))
#define get_pointer(x) get_pointer_mangled(x)
#define get_base(x) get_base_mangled(x)
#define get_length(x) get_length_mangled(x)

#else

typedef void *bp_array;

__attribute__((always_inline)) bp_array packBP(int *pointer, const size_t baseIndex,
											   const size_t sizeInBytes)
{
	if (0 == baseIndex)
	{

		struct bp_array_s *largeDescriptor = malloc(sizeof(struct bp_array_s));
		if (NULL == largeDescriptor)
		{
			exit(EXIT_FAILURE);
		}
		*largeDescriptor = packBP_mangled(pointer, baseIndex, sizeInBytes);

		return (bp_array)largeDescriptor;
	}

	int *descriptor = cheri_setoffset(pointer, 0);
	descriptor = cheri_setbounds(descriptor, sizeInBytes);
	descriptor = cheri_setoffset(descriptor, baseIndex);

	if (baseIndex != cheri_getoffset(descriptor) || sizeInBytes != cheri_getlen(descriptor))
	{
		struct bp_array_s *largeDescriptor = malloc(sizeof(struct bp_array_s));
		if (NULL == largeDescriptor)
		{
			exit(EXIT_FAILURE);
		}
		*largeDescriptor = packBP_mangled(pointer, baseIndex, sizeInBytes);

		return (bp_array)largeDescriptor;
	}

	return (bp_array)descriptor;
}

__attribute__((always_inline)) bool isMangled(bp_array bp)
{
	return 0 == cheri_getoffset(bp);
}

__attribute__((always_inline)) void *get_pointer(bp_array bp)
{
	if (isMangled(bp))
	{
		return cheri_setoffset(get_pointer_mangled(*(struct bp_array_s *)bp), 0);
	}
	void *ret = bp;
	return cheri_setoffset(ret, 0);
}

__attribute__((always_inline)) size_t get_base(bp_array bp)
{
	if (isMangled(bp))
	{
		return get_base_mangled(*(struct bp_array_s *)bp);
	}
	return cheri_getoffset(bp);
}

__attribute__((always_inline)) size_t get_length(bp_array bp)
{
	if (isMangled(bp))
	{
		return get_length_mangled(*(struct bp_array_s *)bp);
	}
	return cheri_getlength(bp);
}

#endif

#ifndef __CHERI_PURE_CAPABILITY__

__attribute__((always_inline)) void callBP_dispatch(void (*function)(bp_array), bp_array args)
{
	function(args);
}

#define callBP(function, pointer, base, length)                                                    \
	callBP_dispatch((function), packBP((pointer), (base), (length)))

#else
__attribute__((always_inline)) void call_and_free(void (*function)(bp_array), void *descriptor)
{
	function(descriptor);
	free(descriptor);
	return;
}

__attribute__((always_inline)) void callBP_dispatch(void (*function)(bp_array), void *pointer,
													size_t base, size_t length)
{
	bp_array descriptor = packBP(pointer, base, length);

	if (0 == base)
	{
		call_and_free(function, descriptor);
		return;
	}

	function(descriptor);
	return;
}

#define callBP(function, pointer, base, length) (callBP_dispatch(function, pointer, base, length))

#endif

#define RUN_LENGTH 64

size_t min(size_t a, size_t b);
bool isSorted(int arr[], size_t length);
void insertionSort(bp_array bp);
void merge(bp_array bp);
void timSort(bp_array bp);

#endif