#include "include/common.h"
#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>

/***
 * The point of this program is to demonstrate the failing of a program
 * that tries to extend the bounderies of a capable pointer.
 ***/
int main()
{
	int32_t array[16] = {0};

	uint32_t bounds = 64;
	puts("bounds(64): ");
	if (0 == scanf("%u", &bounds))
	{
		error("Extraneous input");
	}

	int32_t *custom_boundes_array = cheri_setbounds(array, bounds);

	uint64_t length = cheri_length_get(custom_boundes_array);
	for (uint32_t counter = 0; counter < length / sizeof(int32_t); counter++)
	{
		inspect_pointer(custom_boundes_array + counter);
		// Read value to crash
		printf("Count: %d, Value: %d\n", counter, *(custom_boundes_array + counter));
	}
}
