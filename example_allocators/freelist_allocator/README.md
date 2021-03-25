Segregated freelist allocator implementation,
supporting alloc and dealloc calls for three size classes
(16 bytes, 256 bytes, 4 kilobytes).

Supports per-pointer CHERI bounds protection in malloc with
cheri_bounds_set_exact() on all returned pointers.

In this simple example, the CHERI bounds are used as the
only storage mechanism for each cell's size class. This
relies on the client code not modifying capability metadata
(or at least, using the same capability value that is
returned by alloc as the parameter in the corresponding call
to dealloc).

Execute `make test` to build a client program which
uses this freelist allocator for a binary_trees benchmark,
and execute this on a running QEMU CHERI instance.
