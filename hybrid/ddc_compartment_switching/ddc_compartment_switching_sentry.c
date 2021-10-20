/***
 * This is an adaptation of `ddc_compartment_switching.c` to exemplify wrapping
 * the switching function within a sentry. Note that the code does not
 * **require** the switching code be wrapped in a sentry --- it can be called
 * as any normal function in hybrid mode, without any special security checks.
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

// Simple function accessing the compartment
int compartment_simple_fun();
// The function connected to the assembly trampoline
extern int switch_compartment(void *stack, size_t size);

int main()
{
	uint8_t *simple_block = malloc(5000);
	size_t compartment_size = 2000;
	simple_block[1900] = 80;

	// Make the function within the sentry explicitly executable by
	// inheriting the PCC executable permission
	void *__capability switch_exec =
		cheri_perms_and((void *__capability) switch_compartment,
						cheri_perms_get(cheri_pcc_get()) | CHERI_PERM_EXECUTE);
	// Wrap our function in a sentry
	int (*__capability wrap_fn)(void *, size_t) = cheri_sentry_create(switch_exec);

	assert(cheri_is_valid(wrap_fn));
	assert(cheri_is_sentry(wrap_fn));
	wrap_fn(simple_block, compartment_size);
	return 0;
}

int compartment_simple_fun()
{
	uint8_t *__capability ddc_cap = cheri_ddc_get();
	assert(cheri_tag_get(ddc_cap) && cheri_length_get(ddc_cap) == 2000);
	assert(ddc_cap[1900] == 80);
	return 0;
}
