#include "include/common.h"
#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>

/***
 * The point of this program is to demonstrate the failing of a program
 * that tries to extend the bounderies of a capable pointer.
 * Compared to the simpler 'bounds.c', here we try to modify the bounds
 * explicitly.
 ***/

int main()
{
	int32_t array[16] = {0};
	uint32_t bounds = 64;

	puts("Bounds [Choose a value greater than 64]:");
	if (0 == scanf("%u", &bounds))
	{
		error("Extraneous input");
	}
	// Trying to extend the bounds generates an exception.
	int32_t *custom_boundes_array = cheri_setbounds(array, bounds);
}
