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
