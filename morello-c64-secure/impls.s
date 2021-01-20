// Copyright (c) 2024 The CapableVMs "CHERI Examples" Contributors.
// See COPYRIGHT in the project root for details.
// SPDX-License-Identifier: Apache-2.0 OR MIT


// Note that these demos make use of the fact that gcvalue(c<N>) == x<N>.


        .text
        .balign 4

        // A: C64-Secure, with naive (insecure) csp bounds restriction.
        //
        // Accept that some frames may have a csp that encompasses the frame
        // record. This is the naive solution and does not waste any stack
        // memory, but probably compromises the security properties of
        // C64-Secure.
	.global A
	.type A, @function
A:
	// Frame record:
        //    cfp + 32:  CLR / previous PCC
        //    cfp + 16:  Previous CSP
        //    cfp +  0:  Previous CFP
        mov c16, csp
	str cfp, [csp, #-48]!
	stp c16, clr, [csp, #16]
        // Point cfp to the frame record, and restrict its bounds.
        scbnds cfp, csp, #48

        scoff c16, csp, xzr
        sub x17, fp, x16
        scbnds c17, c16, x17      // Warning: Inexact! May overlap frame record.
        sub x16, sp, w0, uxtw #4  // Allocate locals.
        scvalue csp, c17, x16

1:      mov c0, cfp
        mov c1, csp
        adr c2, 2f
        bl what_next
        // w0: 'locals' for the callee.
        // c1: The callee's address (if any).
2:      chktgd c1
        b.cc 3f
        blrs c1
        b 1b  // Keep going until it's time to return.
3:
        // Epilogue: Unravel everything.
        ldp c16, clr, [cfp, #16]
        ldr cfp, [cfp]
        mov csp, c16
	ret clr
A_end:
        .size A, A_end-A


        // B: C64-Secure, with conservative csp bounds restriction.
        //
        // Add padding below the frame record to ensure that csp does not cover
        // it. This might waste substantial amounts of stack memory, especially
        // near the start of the stack.
	.global B
	.type B, %function
B:
	// Frame record:
        //    cfp + 32:  CLR / previous PCC
        //    cfp + 16:  Previous CSP
        //    cfp +  0:  Previous CFP
        mov c10, csp
	str cfp, [csp, #-48]!
	stp c10, clr, [csp, #16]
        // Point cfp to the frame record, and restrict its bounds.
        scbnds cfp, csp, #48

        // The requested length is gcvalue(cfp) - gcbase(csp).
        // We'll round that down to get the representable length.
        scoff c10, csp, xzr
        sub x11, fp, x10
        // rrlen rounds the length _up_, so use rrmask instead and round the
        // length down manually.
        rrmask x12, x11
        and x12, x11, x12
        // We assume that since the original stack must have been larger than
        // this, its base is suitably aligned. Whilst this is intuitively true,
        // and sufficient for demonstration purpose, we have not proven this
        // property.
        scbndse c10, c10, x12
        sub x12, x12, w0, uxtw #4   // Allocate locals.
        scoff csp, c10, x12

1:      mov c0, cfp
        mov c1, csp
        adr c2, 2f
        bl what_next
        // w0: 'locals' for the callee.
        // c1: The callee's address (if any).
2:      chktgd c1
        b.cc 3f
        blrs c1
        b 1b  // Keep going until it's time to return.
3:
        // Epilogue: Unravel everything.
        ldp c10, clr, [cfp, #16]
        ldr cfp, [cfp]
        mov csp, c10
	ret clr
B_end:
        .size B, B_end-B


        // C: C64-Secure v2, with precisely-restricted csp (and full cfp).
        //
        // Give CFP full-stack bounds, and derive a smaller, exact CSP for each
        // function.
        //
        // This assumes that, on entry, CFP's bounds extend to the stack limit.
        .global C
        .type C, %function
C:
        // Frame record:
        //    cfp + 16: CLR / previous PCC
        //    cfp +  0: Previous CFP with previous CSP's bounds
        mov c16, csp
        scvalue c16, c16, fp
        stp c16, clr, [csp, #-32]!
        // Move the value of csp into cfp, but keep cfp's bounds.
        mov x16, sp
        scvalue cfp, cfp, x16

        // Derive a new csp from the new cfp.
        // NB: For now, we just allow 16KB per function, but we could pick the
        // longest length that guarantees 16-byte alignment (E=4).
        mov x16, #(16 * 1024)
        sub x17, sp, x16
        scvalue c17, cfp, x17
        scbndse c17, c17, x16
        sub x16, sp, w0, uxtw #4    // Allocate locals.
        scvalue csp, c17, x16
        // NB: The very last section of stack will be unusable because
        // scbnds{e} fails if the required bounds exceed the bounds of <Cn>.

1:      mov c0, cfp
        mov c1, csp
        adr c2, 2f
        bl what_next
        // w0: 'locals' for the callee.
        // c1: The callee's address (if any).
2:      chktgd c1
        b.cc 3f
        blrs c1
        b 1b  // Keep going until it's time to return.
3:
        // Epilogue: Unravel everything.
        ldp c16, clr, [cfp]
        add x17, fp, #32        // Drop locals and the stack frame.
        scvalue cfp, cfp, x16
        scvalue c16, c16, x17
        mov csp, c16
        ret clr
C_end:
        .size C, C_end-C


        // Z: AAPCS64-cap, with no csp bounds restriction.
        .global Z
        .type Z, %function
Z:
        // Frame record:
        //    cfp + 16: CLR / previous PCC
        //    cfp +  0: Previous CFP
        stp cfp, clr, [csp, #-32]!
        mov cfp, csp

        // Allocate locals.
        // Morello has no 'sub <Cd>, <Cn>, <Xn>` form, so we have to negate
        // the length explicitly.
        neg x0, x0, lsl #4
        add csp, csp, x0

1:      mov c0, cfp
        mov c1, csp
        adr c2, 2f
        bl what_next
        // w0: 'locals' for the callee.
        // c1: The callee's address (if any).
2:      chktgd c1
        b.cc 3f
        blrs c1
        b 1b  // Keep going until it's time to return.
3:
        // Epilogue: Unravel everything.
        mov csp, cfp
        ldp cfp, clr, [csp], #32
        ret clr
Z_end:
        .size Z, Z_end-Z


        // Helpers for mapping a code address to a given demo function. This is
        // how we identify which ABI was used.
        //
        // Each of these take the address to query in c0. That, and the
        // per-function bounds, are passed in the tail-call to the `is_in_range`
        // helper.

        .global is_in_A
        .type is_in_A, %function
is_in_A:
        adr c10, A
        gcvalue x1, c10
        add x2, x1, #(A_end-A)
        b is_in_range
        .size is_in_A, .-is_in_A


        .global is_in_B
        .type is_in_B, %function
is_in_B:
        adr c10, B
        gcvalue x1, c10
        add x2, x1, #(B_end-B)
        b is_in_range
        .size is_in_B, .-is_in_B


        .global is_in_C
        .type is_in_C, %function
is_in_C:
        adr c10, C
        gcvalue x1, c10
        add x2, x1, #(C_end-C)
        b is_in_range
        .size is_in_C, .-is_in_C


        .global is_in_Z
        .type is_in_Z, %function
is_in_Z:
        adr c10, Z
        gcvalue x1, c10
        add x2, x1, #(Z_end-Z)
        b is_in_range
        .size is_in_Z, .-is_in_Z
