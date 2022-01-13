/* Compartment from which we call the switcher to perform inter-compartment
 * transition. The call is via a capability, to update the PCC bounds
 * appropriately to cover `switch_compartment`.
 */
comp_f_fn:
    // Retrieve local capability containing switcher information
    mrs       c1, DDC
    gclim     x1, c1
    sub       x1, x1, #16
    ldr       c1, [x1]

    // Try to dereference it to retrieve switcher DDC; this is expected to fail
    // due to the local capability being sealed (`main.c:143`).
    ldr       c1, [c1]

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
