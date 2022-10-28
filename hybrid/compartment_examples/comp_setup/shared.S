// Copyright (c) 2021 The CapableVMs "CHERI Examples" Contributors.
// SPDX-License-Identifier: MIT OR Apache-2.0

#include "include/base.h"

.global comp_f_fn

.text
.balign 4

/* This function has been updated with a few features:
 * - it identifies which compartment to switch to based on the value within the
 * `CID_EL0` register;
 * - it reads compartment data from memory, rather than it having to be passed
 * to it via registers - this includes DDC and PCC of compartment to switch to.
 */
.global switch_compartment
.type switch_compartment, "function"
switch_compartment:

    // Get compartment to switch to data
    mrs       c10, CID_EL0
    mov       x11, #COMP_SIZE
    mul       x10, x10, x11

    // Setup SP
    mov       x12, sp
    add       x11, x10, #COMP_OFFSET_STK_ADDR
    ldr       x11, [x19, x11]
    add       x13, x10, #COMP_OFFSET_STK_LEN
    ldr       x13, [x19, x13]
    add       sp, x11, x13

    // Derive a new clr to restore PCC, and store it.
    cvtp      c11, lr

    // Save old DDC, old SP, old LR on stack
    mrs       c10, DDC
    stp       c10, c11, [sp, #-48]!
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

    // Load PCC, including function we are jumping to within compartment
    add       x11, x10, #COMP_OFFSET_PCC
    ldr       c0, [x19, x11]

    // Load DDC
    add       x11, x10, #COMP_OFFSET_DDC
    ldr       c11, [x19, x11]
    msr       DDC, c11

    bl        clean+4

    // Jump to the function within the compartment we are switching to
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

/* The function in this compartment just writes to some memory within its
 * bounds, to ensure it is properly called.
 */
.type comp_f_fn, "function"
comp_f_fn:
    mrs c10, DDC
    mov x11, 42
    str x11, [x10, #4000]

    ret clr


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

