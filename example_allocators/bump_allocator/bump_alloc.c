/**********************************
 * bump_alloc.c
 * Jeremy.Singer@glasgow.ac.uk
 *
 * This is a simple bump-pointer allocator.
 * It mmaps a large buffer of SIZE bytes,
 * then allocates this space in word-sized
 * chunks to client code (in main fn).
 *
 * Initial simple memory allocator test.
 * Explore capability narrowing operations
 * and intrinsics for bound reporting.
 */

#include <cheriintrin.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "bump_alloc.h"

int count = 0;		 /* number of bytes allocated so far*/
int max = 0;		 /* upper limit for count */
char *buffer = NULL; /* the allocation buffer */

void init_alloc(int size_in_bytes)
{
	/* request memory for our allocation buffer
	 * NB mmap min bounds for capability is 1 page (4K)
	 */
	char *res = mmap(NULL, size_in_bytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (res == MAP_FAILED)
	{
		perror("error in initial mem allocation");
		exit(-1);
	}

	buffer = res;
	max = size_in_bytes;
	return;
}

/*
 * allocate len bytes with bump pointer allocator
 * this is our simplistic `malloc` function
 */
char *bump_alloc(int len)
{
	char *chunk = buffer + count;
	size_t rounded_len; /* for CHERI alignment */
	size_t new_count;	/* for buffer overflow check */

	/* ensure we can represent the capability accurately,
	 * see p30 of CHERI C/C++ Prog Guide (June 2020)
	 * www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947
	 */
	chunk = __builtin_align_up(chunk, ~cheri_representable_alignment_mask(len) + 1);
	rounded_len = cheri_representable_length(len);

	new_count = (chunk + rounded_len) - buffer;
	if (new_count > max)
	{
		/* out of bounds - don't allocate anything */
		chunk = 0;
	}
	else
	{
		/* restrict capability range before returning ptr */
		chunk = cheri_bounds_set_exact(chunk, rounded_len);

		/* update bytes allocated count */
		count = new_count;
	}

	return chunk;
}
