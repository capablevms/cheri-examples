/*
 * good_client.c
 * Jeremy Singer
 *
 * Example use of bump allocator, showing how
 * 'good' client code should interact with the
 * allocator.
 */

#include <stdio.h>

#include "bump_alloc.h"
#include "include/common.h"

/* should program generate debugging output? 1 for Yes, 0 for No */
#define DEBUG_PRINTF 1

/* size of buffer to allocate (in words) */
#define NUM_WORDS 100

int main()
{
	int i;

	init_alloc(NUM_WORDS * sizeof(int));

	/* now try to do some bump pointer allocations */
	for (i = 0; i < NUM_WORDS; i++)
	{
		int *x = (int *)bump_alloc(1 * sizeof(int));
		if (DEBUG_PRINTF)
			inspect_pointer(x);
		if (x)
		{
			*x = 42;
			if (DEBUG_PRINTF)
				printf("stored value %d\n", *x);
		}
		else
		{
			if (DEBUG_PRINTF)
				printf("bump_alloc returned null\n");
		}
	}

	return 0;
}
