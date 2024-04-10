// We've found little documentation of `cheri_cap_build`, and on the surface,
// it seems similar to `cheri_address_set`. These functions do similar things,
// but they're different. This example tries to demonstrate what those
// differences are. Briefly summarised…:
//
// - `cheri_address_set()` takes a cap and a ptr, and returns a new cap with
//    the address set to the ptr passed in, derived from the cap passed in.
//    It's basically taking an address and popping it into the appropriate
//    place in an existing capability to create a new one.
// - `cheri_cap_build()` takes a cap and a uintcap_t (a uintptr_t equivalent
//    for capabilities) and returns a new cap with the bits of the uintcap_t,
//    which is valid if and only if it would be no more permissive than the cap
//    passed as the first argument. This is taking all of the data in the
//   `uintcap_t` and creating a cap from it, rather than deriving any of it
//    from the first argument --- which differentiates `cheri_address_set` and
//   `cheri_cap_build`.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/common.h"
#include "cheriintrin.h"

int main()
{
    // This example relies on morello-specific symbols, so we won't run
    // it on anything else.
#if !(defined(__aarch64__) && __ARM_ARCH == 8)
    printf("Skipping; this example only runs on morello.");
#else
    // We create two capabilities: x, and y, which is derived from x. We note
    // that, though we use `cheri_address_set()`, it doesn't actually set (i.e.
    // _change_) the address of its arguments, but construct a new capability
    // with the address we pass.
    //
    // cheri_address_set(void* __capability, uintptr_t) -> void *__capability
    void *__capability x = cheri_ddc_get();
    uintptr_t y_ptr = (uintptr_t) malloc(20);
    void *__capability y = cheri_address_set(x, y_ptr);

    // The second capability printed below is created from the first using
    // `cheri_address_set()`. Notice that `cheri_address_set()` created a new
    // capability rather than setting the address on the first.
    pp_cap(x);
    pp_cap(y);
    printf("\n\n");

    // We create an (invalid!) capability by writing it directly.
    void *my_cap_mem = malloc(16);
    long cap;
    cap = 0xdc5f400000030005;
    memcpy(my_cap_mem + 8, &cap, 8);
    cap = 0x00000000deadbeef;
    memcpy(my_cap_mem + 0, &cap, 8);
    void *__capability my_cap = (void *__capability) *(void *__capability *) my_cap_mem;

    // We build a new capability using `cheri_cap_build()`. Note that this
    // capability is _valid_, because it's derived from `x`, which is valid.
    // The address comes from `my_cap`, which isn't valid, but we can still
    // create a valid capability beecause `cheri_cap_build` just reads it's
    // address bits and constructs a capability.

    // To date we haven't found the specific type signature for this, but at
    // https://www.morello-project.org/resources/morello-linux-morelloie/
    // we can see:
    // void *__builtin_cheri_cap_build(const void *key, unsigned __intcap bits);
    // After experimenting, we've found it wants to be treated as…:
    // cheri_cap_build(void *__capability, uintcap_t) -> void *__capability
    void *__capability valid_cap = cheri_cap_build(x, (uintcap_t) my_cap);

    // The first capability printed below is created by writing its structure
    // into memory; it's not valid, it's just a bunch of bits. The second is an
    // equivalent capability, which is constructed using `cheri_cap_build().
    // Notice that the second one is valid; that's because it's derived using a
    // valid capability (the variable `x`, which contains a copy of the DDC).
    pp_cap(my_cap);
    pp_cap(valid_cap);
    printf("\n\n");

    // It might appear that `cheri_cap_build` could be used to create
    // capabilities with elevated permissions, by writing specific permission
    // bits that we want to elevate before recreating a capability. That's not
    // possible. To demonstrate, the rest of this example uses
    // `cheri_cap_build` to rebuild capabilities from raw bits stored in the
    // heap.

    // Write a copy of the DDC to heap, so we can play with it later
    char *ddc_copy_space = malloc(16);
    void *__capability ddc_copy_val = cheri_ddc_get();
    void **ddc_copy_raw_ptr = (void **) &ddc_copy_val;
    memcpy(ddc_copy_space, ddc_copy_raw_ptr, 16);

    // Switch DDC, removing the mutable load permission, so we can attempt to
    // restore permissions we _shouldn't_ have later on.
    void *__capability newDDC = cheri_perms_and(cheri_ddc_get(), ~ARM_CAP_PERMISSION_MUTABLE_LOAD);
    asm("MSR DDC, %[cap]\n\t" : : [cap] "C"(newDDC) : "memory");

    // Create a capability and copy it into heap; use this to construct two new
    // capabilities, one unmodified and one with restored permissions, both
    // using cheri_cap_build We remove some permissions from the DDC and try to
    // construct a capability that has them again.
    int val = 5;
    char *modspace = malloc(16);
    void *__capability val_cap = (void *__capability) &val;
    void **cap_bytes_raw_ptr = (void **) &val_cap; // a pointer to the cap's bytes
    memcpy(modspace, cap_bytes_raw_ptr, 16);

    // Re-read the capability from heap and attempt to build it again.
    // This works — so long as cheri_cap_build's first argument is a capability
    // as permissive as the second argument, we can create a valid capability
    // using the bits of the second argument.
    // (a `uintcap_t` is the length of a capability)
    void *__capability rebuilt_as_stored = cheri_cap_build(
        cheri_ddc_get(), (uintcap_t) (void *__capability) *(void *__capability *) modspace);
    pp_cap(rebuilt_as_stored);

    // Try to reconsitute the capability in memory with elevated permissions.
    // First, restore all permissions to cap in heap using old DDC
    modspace[14] = ddc_copy_space[14];
    modspace[15] = ddc_copy_space[15];

    // Then use the copy of the DDC in heap to try to reconstruct a capability
    // pointing to `val` with the additional permissions restored. We can't:
    // when casting `ddc_copy_space` back to a capability, we create a
    // capability using our currently installed DDC, which has been altered.
    // That means the first argument doesn't have the permission we're trying
    // to restore, so the second argument is more permissive, and we receive an
    // invalid capability.
    void *__capability rebuilt_constructed_from_ddc_in_heap =
        cheri_cap_build((void *__capability) *(void *__capability *) ddc_copy_space,
                        (uintcap_t) (void *__capability) *(void *__capability *) modspace);
    pp_cap(rebuilt_constructed_from_ddc_in_heap);
    printf("\n");

    // To demonstrate the difference between cheri_cap_build and
    // cheri_address_set, here we remove all permissions on our heap-stored
    // capability and restore it using `cheri_cap_build` and the DDC. Notice
    // that the DDC has many permissions, but the capability we create still
    // has none, because its value is determined by the second argument; _only
    // its validity bit_ is determined by a comparison with the first.
    // Afterwards, we recreate it using `cheri_address_set`, which takes
    // metadata such as permissions from the first argument.
    modspace[14] = 0x00;
    modspace[15] = 0x00;
    void *__capability rebuilt_with_no_perms = cheri_cap_build(
        cheri_ddc_get(), (uintcap_t) (void *__capability) *(void *__capability *) modspace);
    pp_cap(rebuilt_with_no_perms);

    void *__capability rebuilt_using_set_addr = cheri_address_set(
        cheri_ddc_get(), (uintcap_t) (void *__capability) *(void *__capability *) modspace);
    pp_cap(rebuilt_using_set_addr);

    free(ddc_copy_space);
    free(modspace);
    free((void *) y_ptr);
    free(my_cap_mem);
    return 0;
#endif
}
