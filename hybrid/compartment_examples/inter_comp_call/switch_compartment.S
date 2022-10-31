/* The compartment switch function. Expects compartment information to be
 * stored in memory (defined by the capability stored in register `c29`).
 * Performs a compartment switch based on the id saved in `x0` (currently just
 * an integer index into the `comps` array).
 */

#include "include/secure.h"

.global switch_compartment
.global switch_compartment_end

.type switch_compartment, "function"
switch_compartment:
    // Store entering compartment's DDC, and move to memory containing
    // compartment info
    mrs       c2, DDC
    mov       x10, x0

    // Expect switcher DDC in c29
    msr       DDC, c29

    // Get compartment to switch to data
    mov       x11, #COMP_SIZE
    mul       x10, x10, x11

    // Load PCC, including function we are jumping to within compartment
    add       x11, x10, #COMP_OFFSET_PCC
    ldr       c0, [x29, x11]

    // Load DDC
    add       x11, x10, #COMP_OFFSET_DDC
    ldr       c1, [x29, x11]

    // Setup SP
    mov       x12, sp
    add       x11, x10, #COMP_OFFSET_STK_ADDR
    ldr       x11, [x29, x11]
    mov       sp, x11

    // Install compartment DDC
    msr       DDC, c1

    // Save old DDC (c2), old SP (x12), old LR (clr) on stack
    stp       c2, clr, [sp, #-48]!
    str       x12, [sp, #32]

    // Stack layout at this point:
    //
    //     `stack + size` -> ________________________
    //            sp + 40 -> [  <alignment pad>  ]   ^
    //            sp + 32 -> [      old SP       ]   |
    //            sp + 24 -> [ old CLR (hi64)    ]   |
    //            sp + 16 -> [ old CLR (lo64)    ]   |
    //            sp +  8 -> [ old DDC (high 64) ]   | DDC bounds
    //            sp +  0 -> [ old DDC (low 64)  ]   |
    //                                 :             :
    //            `stack` -> ________________________v

    // Clean all registers, except register used to call function within
    // compartment we are transitioning to
    bl        clean+4

    // Jump to the function within the compartment we are switching to (this
    // also sets PCC)
    blr       c0

    // Clean capabilities left in the return value.
    mov       w0, w0
    bl        clean

    // Restore the caller's context and compartment.
    ldp       c10, clr, [sp]
    ldr       x12, [sp, #32]
    msr       DDC, c10
    mov       x10, #0
    mov       sp, x12

    ret       clr

    // Inner helper for cleaning capabilities from registers, either side of an
    // AAPCS64 function call where some level of distrust exists between caller
    // and callee.
    //
    // Depending on the trust model, this might not be required, but the process
    // is included here for demonstration purposes. Note that if data needs to
    // be scrubbed as well as capabilities, then NEON registers also need to be
    // cleaned.
    //
    // Callers should enter at an appropriate offset so that live registers
    // holding arguments and return values (c0-c7) are preserved.
clean:
    mov x0, #0
    mov x1, #0
    mov x2, #0
    mov x3, #0
    mov x4, #0
    mov x5, #0
    mov x6, #0
    mov x7, #0
    mov x8, #0
    mov x9, #0
    mov x10, #0
    mov x11, #0
    mov x12, #0
    mov x13, #0
    mov x14, #0
    mov x15, #0
    mov x16, #0
    mov x17, #0
    // x18 is the "platform register" (for some platforms). If so, it needs to
    // be preserved, but here we assume that only the lower 64 bits are
    // required.
    mov x18, x18
    // x19-x29 are callee-saved, but only the lower 64 bits.
    mov x19, x19
    mov x20, x20
    mov x21, x21
    mov x22, x22
    mov x23, x23
    mov x24, x24
    mov x25, x25
    mov x26, x26
    mov x27, x27
    mov x28, x28
    mov x29, x29  // FP
    // We need LR (x30) to return. The call to this helper already cleaned it.
    // Don't replace SP; this needs special handling by the caller anyway.
    ret
switch_compartment_end:

