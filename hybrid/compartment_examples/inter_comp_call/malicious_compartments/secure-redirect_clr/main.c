/* This example attempts to set a `clr` (`cvtp` instruction in `comp_f_fn` in
 * `compartments.S`) which would jump to a malicious function
 * (`comp_f_malicious` in `compartments.S`) after `switch_compartment` returns.
 * The execution faults when entering the malicious function via returning from
 * `switch_compartment` (final `ret` of `switch_compartment()`) as the bounds
 * on the capability loaded into `clr` are inherited from the PCC of the
 * compartment via the `cvtp` instruction, being the only way the compartment
 * itself can create new capabilities from scratch (assuming no access to other
 * capabilities).
 *
 * Note that `compartments.S` refers to `compartments-redirect_clr.S`.
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
 * - the size of the stack (8B)
 * - the size of the heap (8B)
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

    // Ensure 16-byte alignment throughout the compartment bounds
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
