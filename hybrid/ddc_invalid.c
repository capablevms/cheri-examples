/***
 * This program causes an "In-address space security Exception" by clearing the
 * tag of the capability in the DDC
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
	// Before clearing the tag we ensure we have a valid one
	assert(cheri_tag_get(cheri_ddc_get()));
	// Clearing the tag will cause the exception
	write_ddc(cheri_tag_clear(cheri_ddc_get()));
	return 0;
}