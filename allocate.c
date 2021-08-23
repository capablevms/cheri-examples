#include "include/common.h"
#include <stdio.h>
#include <stdlib.h>

/***
 * Inspect capabilities metadata after a malloc.
 ***/

int main()
{
	size_t size = 31;

	printf("Size: ");
	if (0 == scanf("%lu", &size))
	{
		error("Extraneous input");
		return -1;
	}

	void *memory = malloc(size);
	pp_cap(memory);
}
