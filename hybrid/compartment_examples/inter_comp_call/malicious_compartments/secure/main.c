/* In this example, we tighten security even further, building on previous
 * examples [1]. We store the DDC and the PCC of `switch_compartment`
 * consecutively within its own bounds. Then, we create a capability pointing
 * to the DDC, which is sealed such that it can only be used in an `lpb`-type
 * call, and provide local copies of this capability to each compartment. Thus,
 * compartments are allowed to call `switch_compartment` via `ldpblr`, without
 * access to either its PCC or DDC (by nature of capabilities).
 *
 * The local copies are stored in the heap space of each compartment (as we do
 * not implement memory management at this point, this has no bearing at
 * present, but most likely will in the future). This could essentially be
 * considered a separate region of memory. Further, when using the local
 * capabilities to call `switch_compartment` from a compartment, the
 * destination register *must* be `c29`.
 *
 * Note that in the current design, each compartment has a single function it
 * executes when it is called. Allowing multiple entry points can be modelled
 * by having more capabilities be saved, in addition to the compartment
 * switching one, in each compartment's heap. Alternatively, we could, within
 * the compartment switcher, determine in which compartment we need to switch
 * to call a given function, based on PCC bounds for each compartment.

 * Additionally, we also changed the expected memory layout in the
 * compartments. In the absence of a custom memory allocator, we note that the
 * heap is not used, except for manually storing the local capability copy as
 * detailed above. Current compartment memory layout:
 *
 *       -----------------  < DDC + total_comp_size
 *      |  switcher_call  |
 *      |                 |
 *      |       ^         |
 *      |      HEAP       |
 * sp > |-----------------| < DDC + comp_stack_size
 *      |     STACK       |
 *      |       v         |
 *       -----------------  < `DDC value`
 *
 * [1]
 https://github.com/capablevms/cheri-examples/tree/master/hybrid/compartment_examples/inter_comp_call
 */

#include "../../../../../include/common.h"
#include "../../../../include/utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

/*******************************************************************************
 * Assembly Functions & Consts
 ******************************************************************************/

extern void executive_switch(void *__capability);
extern int switch_compartment();
extern void comp_f_fn();
extern void comp_g_fn();
extern void comp_f_fn_end();
extern void comp_g_fn_end();
extern void switch_compartment_end();

/*******************************************************************************
 * Types & Consts
 ******************************************************************************/

const size_t comp_stack_size = 2000;
const size_t total_comp_size = 4992;
size_t id = 0;

/* Capabilities representing DDC and PCC of the switcher, stored in
 * `switcher_caps`. They are saved successively to facilitate calling via
 * `ldpblr`, with the first being the DDC, and the second, the PCC.
 */
const size_t switcher_caps_count = 2;
void *__capability switcher_caps[switcher_caps_count];

/* Capability required by compartments to call `switch_compartment` securely.
 * Duplicated in each compartment. */
void *__capability switcher_call;

/* Abstract representation of a compartment. Within 80 bytes we represent:
 * - an id (8B)
 * - address to the start of the compartment memory area (also lowest address
 *   of the stack) (8B)
 * - address to the start (highest address) of the compartment's stack
 *   (corresponding to the lowest address of the heap) (8B)
 *   the size of the stack (8B)
 * - address to the top (highest addressof the compartment's heap (8B)
 *   the size of the heap (8B)
 * - alignment padding (8B)
 * - the ddc corresponding to the compartment (16B)
 * - the function within the compartment to be executed upon switching (16B)
 */
struct comp
{
	size_t id;
	void *compartment_start;
	void *stack_addr;
	size_t stack_len;
	size_t heap_len;
	void *__capability ddc;
	void *__capability comp_fn;
};

// ASM offsets, included here for validation
#include "../include/secure.h"

static_assert(COMP_SIZE == sizeof(struct comp), "Invalid `COMP_SIZE` provided");
static_assert(COMP_OFFSET_STK_ADDR == offsetof(struct comp, stack_addr),
			  "Invalid `COMP_OFFSET_STK_ADDR` provided.");
static_assert(COMP_OFFSET_STK_LEN == offsetof(struct comp, stack_len),
			  "Invalid `COMP_OFFSET_STK_LEN` provided.");
static_assert(COMP_OFFSET_DDC == offsetof(struct comp, ddc), "Invalid `COMP_OFFSET_DDC` provided.");
static_assert(COMP_OFFSET_PCC == offsetof(struct comp, comp_fn),
			  "Invalid `COMP_OFFSET_PCC` provided.");

struct comp comps[COMP_COUNT];

/*******************************************************************************
 * Privileged Functions
 ******************************************************************************/

/* Create and save required capabilities. Currently, this means:
 * - PCC of `switch_compartment`
 * - DDC of `switch_compartment`
 * - capability to allow compartments to call `switch_compartment` via `lpdblr`
 */
void init_comps()
{
	void *__capability switch_cap = (void *__capability) switch_compartment;
	size_t switcher_size = (uintptr_t) switch_compartment_end - (uintptr_t) switch_compartment;
	switch_cap = cheri_bounds_set(switch_cap, switcher_size);
	switcher_caps[1] = switch_cap;

	void *__capability comps_addr = (void *__capability) &comps;
	comps_addr = cheri_bounds_set(comps_addr, COMP_COUNT * COMP_SIZE);
	switcher_caps[0] = comps_addr;

	switcher_call = (void *__capability) switcher_caps;
	// Seal this capability to be only used via a `lpb` type call
	asm("seal %w0, %w0, lpb" : "+r"(switcher_call) :);
}

void add_comp(uint8_t *_start_addr, void (*_comp_fn)(), void *_comp_fn_end)
{
	assert(id < COMP_COUNT);
	struct comp new_comp;
	new_comp.id = id;

	new_comp.compartment_start = (void *) _start_addr;
	new_comp.stack_addr = (void *) (_start_addr + comp_stack_size);
	new_comp.stack_len = comp_stack_size;
	new_comp.heap_len = total_comp_size - comp_stack_size;

	// Ensure 16-byte alignment throught the compartment bounds
	assert(((uintptr_t) new_comp.compartment_start) % 16 == 0);
	assert(((uintptr_t) new_comp.stack_addr) % 16 == 0);
	assert(total_comp_size % 16 == 0);

	// When creating a compartment, store a local copy of the capability which
	// will allow us to call `switch_compartment` in the heap of the compartment.
	void *heap_top = (void *) (_start_addr + total_comp_size - sizeof(void *__capability));
	memcpy(heap_top, &switcher_call, sizeof(void *__capability));

	void *__capability comp_ddc = (void *__capability) _start_addr;
	comp_ddc = cheri_bounds_set(comp_ddc, total_comp_size);
	new_comp.ddc = comp_ddc;

	// Set up a capability pointing to the function we want to call within the
	// compartment. This will be loaded as the PCC when the function is called.
	void *__capability comp_fn = (void *__capability) _comp_fn;
	size_t comp_fn_size = (uintptr_t) _comp_fn_end - (uintptr_t) _comp_fn;
	comp_fn = cheri_bounds_set(comp_fn, comp_fn_size);
	new_comp.comp_fn = comp_fn;

	comps[id] = new_comp;
	++id;
}

/*******************************************************************************
 * Main
 ******************************************************************************/

int main()
{
	init_comps();

	uint8_t *comp_f = malloc(total_comp_size);
	add_comp(comp_f, comp_f_fn, &comp_f_fn_end);
	uint8_t *comp_g = malloc(total_comp_size);
	add_comp(comp_g, comp_g_fn, &comp_g_fn_end);

	executive_switch(switcher_caps[0]);

	// Check compartment did indeed execute
	assert(comp_g[4000] == 42);
}
