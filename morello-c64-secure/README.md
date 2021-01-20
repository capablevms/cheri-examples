# Morello C64-Secure Example

This example demonstrates a few procedure-call standard variants that provide
partial compartmentalisation with minimal cost.

Full compartmentalisation usually requires register banking and explicit
management of capabilities over the trust boundary. Often, an intermediary is
required, such as a compartment manager. Such approaches have inherent code-size
and performance costs, and so a cheaper (but imperfect) alternative might be
preferred for routine function calls.

For routine function calls, we consider the case where callee is not
deliberately malicious, but might be susceptible to data-driven attacks. Such
attacks should not have arbitrary access to the frame record (which could be
used to divert control flow), nor to the caller's stack.

Three ABIs, with this goal, are demonstrated:

  A: "C64-Secure" (naive variant)

    Restrict csp to the stack frame, and cfp to the frame record itself. Both
    capabilities must be saved (alongside clr), so the frame record has three
    entries in this variant.

    This is a weak compartment because it relies on the generated code not
    accessing cfp at all, other than in the function prologue and epilogue. This
    can often be guaranteed, but not always.

    This variant is simple, but its security is limited by the representable
    precision. If the stack bounds are large (as they typically are), the
    callee's stack will typically end up covering the whole stack anyway, since
    `scbnds` is used, rather than `scbndse`. However, this variant could be
    effective on threads or other subsystems with small stacks.

    In the interactive demo: the bounds on csp don't actually appear to be
    restricted unless you request very large stack frames.

  B: "C64-Secure" (conservative variant)

    As A, but pad stack frames to ensure that they don't overlap. This
    guarantees the expected security proprties, but wastes a lot of space at the
    start of the stack.

    In the interactive demo: the bounds are precise, as expected, and csp never
    grants access to the frame record, but there are a large number of (padding)
    locals on each stack frame.

  C: "C64-Secure v2"

    Attempt to get the security properties of B, but with no wasted space.

    - Leave cfp unrestricted (spanning the whole stack).
    - Restrict csp to the current function's stack frame only.

    Restricting csp to only a single function's frame (rather than all of the
    remaining stack space) means that it can be much more precisely bounded.
    In the function prologue, a new csp is derived from the caller's cfp.

    As a bonus, only a two-entry stack frame is required. One slot holds clr (as
    usual), and the other holds the caller's csp bounds and the cfp address.

    A disadvantage of this variant is that more code is required, but this could
    be simplified if frame sizes are known to be small. The example makes a
    couple of simplifying assumptions (described in the code).

  Z: "AAPCS64-cap"

   This is the procedure-call standard used by C code compiled for Morello
   purecap. It is provided here for reference.

It is not easy to call between some of these variants, so the example won't
provide all options. In practice, it would be possible to convert using a
trampoline of some sort, but that isn't implemented here.
