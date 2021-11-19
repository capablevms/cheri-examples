/***
 * This example showcases two particular actions to help compartmentalization:
 *      - reducing the bounds of the PCC (shared.s:82)
 *      - calling a sentry from within a bounded compartment (shared.s:114)
 *
 * One difficulty, and capability leak, is how to return from the sentry
 * function back to the caller with correct capabilities (i.e., PCC). In this
 * example, we use `clr`, and manually return to it (main.c:49). However,
 * having the `clr` available to the sentry function means it could access
 * capabilities it should not be privy to (e.g., if the bounds of the sentry
 * are set to only cover the specific function). In this example, this is
 * further aggravated by the jump between assembly and `C`: the `C` function is
 * compiled to assembly which makes use of `lr` (which stores pointers, not
 * capabilities), meaning we would, by default, return to a context in which we
 * have no execution permissions (more specifically, the `PCC` is set to cover
 * the callee, and when we return to the caller, the `PCC` is not affected due
 * to returning to a pointer).
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

// Trampoline function to restore caller's PCC after we return from a sentry
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

	// The `add` instruction is required to correctly reset the `sp`, which is
	// modified by the assembled version of this function; this might be
	// compiler-specific. In this example, `sp` is used to store information to
	// reset the environment, therefore we must ensure it is reset correctly
	// upon return.
	//
	// The `ret` instruction produced by
	// default only uses `lr`, instead of `clr`; using the `lr` means we would
	// return with a PCC bound to this function, and not be able to execute
	// within the caller. Using `clr` ensures the PCC is reset correctly.
	//
	// Overall, this example shows it is not a good idea to call C functions
	// via sentries in hybrid mode.
	//
	// NOTE: As the `clr` remains in scope during the execution of this
	// function, it essentially leaks the executable capability. The current
	// design, and the interfacing between `C` and assembly requires this to be
	// available in this function
	asm("add sp, sp, #0x10; ret clr");
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

	// Set the bounds on the sentry, which restricts the PCC when executing the
	// function. The value `40` here was determined by disassembling
	// `comp_fun_c`.
	comp_fun_c_sentry = cheri_bounds_set(comp_fun_c_sentry, 40);

	switch_compartment(comp_mem, comp_size, call_cap, comp_fun_c_sentry);
	//
	// Check the compartment function has been executed.
	assert(comp_mem[1800] == 80);
	return 0;
}
