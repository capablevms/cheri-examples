#include "util.h"

#include <cheriintrin.h>

#include <cheri/cheric.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

static void *create_mmap(mmap_test_case_t test)
{
    printf("creating mapping (%s, fd = %d)\n", test.details, test.fd);

    void *addr = mmap(NULL, MMAP_SIZE, test.prot, test.flags, test.fd, 0);

    if (addr == MAP_FAILED)
    {
        perror("failed to mmap");
        return NULL;
    }

    printf("mmap succeeded (requested length = %d, actual length = %zu): %#lp\n", MMAP_SIZE,
           cheri_getlength(addr), addr);

    return addr;
}

int run_mmap_test(mmap_test_case_t test)
{
    uintptr_t *addr = (uintptr_t *) create_mmap(test);

    int x = 0;
    uintptr_t cap = (uintptr_t) &x;
    *addr = cap;

    printf("original capability: %#lp\n", (void *) cap);
    printf("stored capability: %#lp\n", (void *) *addr);

    if (munmap(addr, MMAP_SIZE))
    {
        perror("failed to unmap");
        return 1;
    }

    return 0;
}
