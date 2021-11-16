/***
 * This example showcases two particular actions to help compartmentalization:
 *      - reducing the bounds of the PCC (shared.s:82)
 *      - calling a sentry from within a bounded compartment (shared.s:114)
 * In addition to the above, we note that an implicit PCC unbounding is
 * performed when a sentry is called in hybrid mode. The PCC is then **not**
 * implicitly rebound. This example uses a trampoline function `comp_fun_tr`
 * which does this rebounding.
 ***/

#include "../../../include/common.h"
#include "../../include/utils.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

// Trampoline function to ensure PCC is rebounded after we return from a sentry
// call; this assembly function also includes the sentry containing the
// function to be called from the container
extern int comp_fun_tr();
// The function connected to the assembly trampoline
extern int switch_compartment(void *stack, size_t size, void *__capability fn_call_start,
							  void *__capability pcc);

// Function to be called via a sentry from within a restricted compartment. As
// the `stk` variable is within DDC, it can write to it.
void comp_fun_c(uint8_t *stk)
{
	stk[1800] = 80;
}

int main()
{
	uint8_t *comp_mem = malloc(5000);
	size_t comp_size = 2000;

	// Create a capability which we will use to tightly bound the PCC for the
	// compartment
	void *__capability call_cap = (void *__capability) comp_fun_tr;
	call_cap = cheri_bounds_set(call_cap, comp_size);

	// Derive a sentry from the PCC to allow calling a function outside PCC
	// bounds
	void *__capability comp_fun_c_sentry = cheri_pcc_get();
	comp_fun_c_sentry = cheri_address_set(comp_fun_c_sentry, (unsigned long) &comp_fun_c);

	switch_compartment(comp_mem, comp_size, call_cap, comp_fun_c_sentry);
	assert(comp_mem[1800] == 80);
	return 0;
}
