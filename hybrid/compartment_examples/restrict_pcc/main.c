/***
 * This example showcases how to restrict a PCC (`shared.s:81`), and that
 * attempting to call a function (`shared.s:103`) lying outside the bounds of
 * the restriced PCC yields an `In-address space security exception`.
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

// Wrapper function to be called from within the restricted compartment, within
// PCC bounds.
extern int comp_fun();
// The function connected to the assembly trampoline.
extern int switch_compartment(void *stack, size_t size, void *__capability fn_call_start,
                              void *__capability pcc);

// Function outside of PCC bounds to be called from within the compartment.
void comp_fun_c(uint8_t *stk)
{
    // unreachable
}

int main()
{
    uint8_t *comp_mem = malloc(5000);
    size_t comp_size = 2000;

    // Create a capability which we will use to tightly bound the PCC for the
    // compartment.
    void *__capability call_cap = (void *__capability) comp_fun;
    call_cap = cheri_bounds_set(call_cap, comp_size);

    switch_compartment(comp_mem, comp_size, call_cap, comp_fun_c);
    assert(false && "Should not get here");
    return 0;
}
