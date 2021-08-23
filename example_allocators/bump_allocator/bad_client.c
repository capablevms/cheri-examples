/*
 * bad_client.c
 * Jeremy Singer
 *
 * Example use of bump allocator, showing how
 * 'bad' client code might interact with the
 * allocator, accessing a pointer out of bounds.
 *
 * Error reported on QEMU-RISCV-BSD is:
 *  In-address space security exception (core dumped)
 *
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
	int *p;

	init_alloc(NUM_WORDS * sizeof(int));

	p = (int *)bump_alloc(1 * sizeof(int));
	if (DEBUG_PRINTF)
		pp_cap(p);
	if (p)
	{
		*p = 42;
		/* first bad op: write outside the pointer bounds */
		*(p + 1) = 0xbaadf00d;
		/* second bad op: read outside the pointer bounds */
		printf("out-of-bounds read: %d\n", *(p + 1));
	}

	return 0;
}
