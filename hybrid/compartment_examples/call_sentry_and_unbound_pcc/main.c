/***
 * This example showcases how a restricted PCC, after being unbound to
 * perform a sentry call, remains unbound, allowing a compartment to access
 * code it should not be privy to. Particularly, after calling `comp_fun_c` via
 * a sentry at `shared.s:110`, we are able to call `comp_fun_c` directly (e.g.,
 * `shared.s:89`).
 * ***/

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
extern int comp_fun();
// The function connected to the assembly trampoline
extern int switch_compartment(void *stack, size_t size, void* __capability fn_call_start, void* __capability pcc);

// Function to be called via a sentry from within a restricted compartment. As
// the `stk` variable is within DDC, it can write to it.
void
comp_fun_c(uint8_t* stk)
{
    stk[1800] += 1;
}

int main()
{
	uint8_t* comp_mem = malloc(5000);
	size_t comp_size = 2000;
    comp_mem[1800] = 1;

    // Create a capability which we will use to tightly bound the PCC for the
    // compartment
    void* __capability call_cap = (void* __capability) comp_fun;
    call_cap = cheri_bounds_set(call_cap, comp_size);

    // Derive a sentry from the PCC to allow calling a function outside PCC
    // bounds
    void* __capability comp_fun_c_sentry = cheri_pcc_get();
    comp_fun_c_sentry = cheri_address_set(comp_fun_c_sentry, (unsigned long) &comp_fun_c);

    switch_compartment(comp_mem, comp_size, call_cap, comp_fun_c_sentry);
    printf("%d\n", comp_mem[1800]);
	return 0;
}
