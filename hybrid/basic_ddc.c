/***
 * This program shows a very basic level of compartmentalisation. It first
 * `malloc` two blocks, and then flip the DDC to point from one to the other.
 ***/

#include "../include/common.h"
#include "include/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

int main()
{
	uint16_t *some_int_ptr = (uint16_t *) malloc(sizeof(uint16_t));
	*some_int_ptr = 200;
	assert(cheri_address_get(cheri_ddc_get()) != some_int_ptr);
	uint32_t *some_other_int_ptr = (uint32_t *) malloc(sizeof(uint32_t));
	write_ddc((void *__capability) some_other_int_ptr);
	assert(cheri_address_get(cheri_ddc_get()) != some_int_ptr);
	assert(cheri_address_get(cheri_ddc_get()) == some_other_int_ptr);
	// Note: this program is very simple and writing to the DDC in this fashion
	// would cause a crash if the program were to execute much further.
	write_ddc((void *__capability) some_int_ptr);
	assert(cheri_address_get(cheri_ddc_get()) != some_other_int_ptr);
	assert(cheri_address_get(cheri_ddc_get()) == some_int_ptr);
	return 0;
}