/***
 * This program is a fusion of ``bounds.c``
 * and ``set_bounds.c``.
 * CHERI limits the range of addresses that may be dereferenced
 * and reading the value outside the range will always give
 * an "In-address space security exception". However, if we try to
 * extend the bounderies of a capable pointer by setting them explicitly,
 * the program fails on "riscv64".
 ***/

#include "include/common.h"
#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int32_t array[12] = {0};
	int32_t *typed_array = &array;
	u_int32_t bounds = 48;

#if defined(__aarch64__) && __ARM_ARCH == 8
	// Do a dereference
	uint64_t length = cheri_getlength(typed_array);
	for (uint32_t counter = 0; counter <= (length / sizeof(int32_t)) + 11; counter++)
	{
		inspect_pointer(typed_array + counter);
		// Read value to crash
		if (counter == 12)
		{
			printf("--> On Morello (ARMv8) dereferencing outside the range"
				   " causes the following exception:\n");
			fflush(stdout);
		}
		printf("Count: %d, Value: %d\n", counter, *(typed_array + counter));
	}
#elif defined(__riscv)
	// Simply increase the bounds
	printf("Bounds [Choose a value greater than 48]:\n");
	if (0 == scanf("%u", &bounds))
	{
		error("Extraneous input");
	}
	printf("Explicitly setting the bounds outside the range causes the following exception: ");
	fflush(stdout);
	int32_t custom_bounds_array = cheri_setbounds(array, bounds);
#else
#error Platform not currently supported.
#endif
}