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
    // Clean up x20 after returning from compartment switch
    mov      x13, #0

    // Try to set the CLR to jump to a malicious function
    adr       x10, comp_f_malicious
    cvtp      clr, x10

    // Retrieve local capability containing switcher information for `pdlbr`
    // instruction (DDC is used as it contains the address where the capability
    // is stored in this particular example)
    mrs       c1, DDC
    gclim     x1, c1
    sub       x1, x1, #16
    ldr       c1, [x1]
    // We use `ldpbr` to not overwrite the `clr`, and use our maliciously
    // installed one
    ldpbr     c29, [c1]

    ret       clr
comp_f_fn_end:

comp_f_malicious:
    mov       x0, #2000
    ret

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
