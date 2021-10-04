/***
 * This example shows an intermediate level of compartment creation.
 * It allocates a block of memory (5000 bytes) but only the first
 * 2000 are accessible by `compartment_simple_fun`.
 *
 * This program raises an exception because we try to access an
 * `int32_t` which is out of the compartment's bounds.
 ***/

#include "../../include/common.h"
#include "../include/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

int compartment_simple_fun();
extern int switch_compartment(void *stack, size_t size);

int main()
{
	uint8_t *simple_block = malloc(5000);
	size_t compartment_size = 2000;
	simple_block[2500] = 8;
	switch_compartment(simple_block, compartment_size);
	return 0;
}

int compartment_simple_fun()
{
	uint8_t *__capability ddc_cap = cheri_ddc_get();
	// This function can access only 2000 bytes, i.e. `compartment_size`
	// So the following will go over its compartment bounds
	ddc_cap[2500] = 12;
	return 0;
}