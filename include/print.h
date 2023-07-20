#ifndef __CHERI_EXAMPLE_PRINT_H
#define __CHERI_EXAMPLE_PRINT_H 

#include <assert.h>
#include <cheri.h>
#include <cheri/cheri.h>
#include <cheri/cheric.h>
#include <machine/sysarch.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// 
#define cheri_tag_get(x)    __builtin_cheri_tag_get(x)
#define cheri_address_get(x) __builtin_cheri_address_get(x)
#define cheri_is_sealed(x) __builtin_cheri_sealed_get(x)
#define cheri_flags_get(x) __builtin_cheri_flags_get(x)

/*
 * Print information about a capability
 */

void print_cap(void *__capability ptr)
{
    uint64_t length = cheri_length_get(ptr);
    uint64_t address = cheri_address_get(ptr);
    uint64_t base = cheri_base_get(ptr);
    uint64_t flags = cheri_flags_get(ptr);
    uint64_t perms = cheri_perms_get(ptr);
    uint64_t type = cheri_type_get(ptr);
    bool tag = cheri_tag_get(ptr);
    uint64_t offset = cheri_offset_get(ptr);

    printf("Capability: %#lp\n", ptr);
    printf("Tag: %d, Perms: %04lx, Type: %lx, Address: %04lx, Base: %04lx, End: %04lx, Flags: %lx, "
           "Length: %04lx, Offset: %04lx\n\n",
           tag, perms, type, address, base, base + length, flags, length, offset);
}

#endif
