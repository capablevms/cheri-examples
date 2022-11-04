/* Compartment from which we call the switcher to perform inter-compartment
 * transition. The call is via a capability, to update the PCC bounds
 * appropriately to cover `switch_compartment`.
 */

// Compartment functions
.global comp_f_fn
.global comp_g_fn

// Labels for size computations
.global comp_f_fn_end
.global comp_g_fn_end

.type comp_f_fn, "function"
comp_f_fn:
    // Set compartment ID we want to switch to
    mov       x0, #1

    // Store the `clr` for exitting `comp_f_fn`; this is overwritten by
    // `switch_compartment`.
    str       clr, [sp, #-16]!

    // Attempt to modify DDC of `switch_compartment`
    mrs       c1, DDC
    ldr       c2, [x20]
    gcvalue   x3, c1
    scvalue   c2, c2, x3
    str       c2, [x20]

    // Retrieve local capability containing switcher information for `ldpblr`
    // instruction (DDC is used as it contains the address where the capability
    // is stored in this particular example)
    gclim     x1, c1
    sub       x1, x1, #16
    ldr       c1, [x1]
    // set leak_x20
    mov       x13, #0
    ldpblr    c29, [c1]

    ldr       clr, [sp], #16

    ret       clr
comp_f_fn_end:

/* The function in this compartment just writes to some memory within its
 * bounds, to ensure it is properly called.
 */
.type comp_g_fn, "function"
comp_g_fn:
    mrs       c10, DDC
    mov       x11, 42
    str       x11, [x10, #4000]

    ret clr
comp_g_fn_end:
