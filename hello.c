#include "include/common.h"
//#include <cheri/cheric.h>
#include <cheriintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/***
 * This is my scratch file for testing things
 ***/
int main()
{
	int32_t array[16] = {0};
	
	int64_t type = 4;
	int32_t* typed_array = &array;
	int32_t* aptr = &array;	
	
	uint64_t length = cheri_length_get(typed_array);
	for (uint32_t counter = 0; counter < length / sizeof(int32_t) + 16; counter++)
	{
		inspect_pointer(typed_array + counter);
		// Read value to crash
		// printf("Count: %d, Value: %d\n", counter, *(typed_array + counter));
	}
}
