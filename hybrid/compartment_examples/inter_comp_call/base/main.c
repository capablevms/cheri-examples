/* This example adds a second compartment, and shows a possible design to
 * achieve inter-compartment switching without (much) privilege escalation. In
 * this file, we setup compartment information within array `comps`
 * (`main.c:84`). This information will be used by the assembly function
 * `switch_compartment`. Further, we save a capability to
 * `switch_compartment()` in a register (`main.c:102`), to be able to call it
 * from a bounded-PCC context, as well as a capability defining the area of
 * memory with compartment information (`main.c:101`). These could be added to
 * the memory area of each created compartment for better security, as well as
 * sealed.
 *
 * Our two compartments provide one entry point each, `comp_f_fn`
 * (`shared.S:93`), and `comp_g_fn` (`shared.S:109`). The design is that
 * compartment `f` will perform a switch to compartment `g`, which performs
 * some observable action (in this instance, sets a specific memory location to
 * a specific integer value). The switching process in (almost) entirely bound
 * within `switch_compartment`, on the execution side, and the memory area with
 * compartment information, for memory access. We note almost, as the
 * executable area is approximated (`main.c:96`).
 */

#include "../../../../include/common.h"
#include "../../../include/utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

/*******************************************************************************
 * Assembly Functions & Consts
 ******************************************************************************/

extern int switch_compartment();
extern void comp_f_fn();
extern void comp_g_fn();

/*******************************************************************************
 * Types & Consts
 ******************************************************************************/

const size_t comp_stack_size = 2000;
const size_t total_comp_size = 5000;
size_t id = 0;

/* Abstract representation of a compartment. Within 80 bytes we represent:
 * - an id (8B)
 * - address to the start of the compartment's stack (8B)
 *   the size of the stack (8B)
 * - address to the start of the compartment's heap (8B)
 *   the size of the heap (8B)
 * - alignment padding (8B)
 * - the ddc corresponding to the compartment (16B)
 * - the function within the compartment to be executed upon switching (16B)
 */
struct comp
{
	size_t id;
	void *stack_addr;
	size_t stack_len;
	void *heap_addr;
	size_t heap_len;
	void *__capability ddc;
	void *__capability comp_fn;
};

// ASM offsets, included here for validation
#include "../include/base.h"

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

/* This function stores the compartment data in memory, so that it can be
 * accessed by the compartments as needed.
 */
void executive_switch(struct comp c)
{
	void *__capability switch_cap = (void *__capability) switch_compartment;
	switch_cap = cheri_bounds_set(switch_cap, 80 * 4);

	void *__capability comps_addr = (void *__capability) &comps;
	comps_addr = cheri_bounds_set(comps_addr, COMP_COUNT * COMP_SIZE);

	asm("mov c19, %w0\n\t"
		"mov c20, %w1\n\t"
		"mov x0, #0\n\t"
		"msr CID_EL0, c0"
		:
		: "r"(comps_addr), "r"(switch_cap));

	switch_compartment();
}

void add_comp(uint8_t *_start_addr, void (*_comp_fn)())
{
	assert(id < COMP_COUNT);

	struct comp new_comp;
	new_comp.id = id;

	new_comp.stack_addr = (void *) _start_addr;
	new_comp.stack_len = comp_stack_size;
	new_comp.heap_addr = (void *) (_start_addr + comp_stack_size);
	new_comp.heap_len = total_comp_size - comp_stack_size;

	void *__capability comp_ddc = (void *__capability) _start_addr;
	comp_ddc = cheri_bounds_set(comp_ddc, total_comp_size);
	new_comp.ddc = comp_ddc;

	// Set up a capability pointing to the function we want to call within the
	// compartment. This will be loaded as the PCC when the function is called.
	void *__capability comp_fn = (void *__capability) _comp_fn;
	comp_fn = cheri_bounds_set(comp_fn, 40);
	new_comp.comp_fn = comp_fn;

	comps[id] = new_comp;
	++id;
}

/*******************************************************************************
 * Main
 ******************************************************************************/

int main()
{
	uint8_t *comp_f = malloc(total_comp_size);
	add_comp(comp_f, comp_f_fn);
	uint8_t *comp_g = malloc(total_comp_size);
	add_comp(comp_g, comp_g_fn);

	executive_switch(comps[0]);

	// Check compartment did indeed execute
	assert(comp_g[4000] == 42);
}
