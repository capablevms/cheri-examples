/***
 * This simple program shows how CHERI limits the
 * range of addresses that may be dereferenced.
 * Reading the value outside the range will give
 * an "In-address space security exception".
 ***/

#include "include/common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int32_t array[16] = {0};
	int32_t *typed_array = &array;

	uint64_t length = cheri_length_get(typed_array);
	for (uint32_t counter = 0; counter <= (length / sizeof(int32_t)) + 15; counter++)
	{
		inspect_pointer(typed_array + counter);
		// Read value to crash
		printf("Count: %d, Value: %d\n", counter, *(typed_array + counter));
	}
}
