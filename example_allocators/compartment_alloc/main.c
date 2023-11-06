#include "../../include/common.h"
#include "compartment_alloc.c"

int main()
{
    // Create capabilities which the heap compartments will construct capabilities against
    // (as if they were the DDC when the allocation is made).
    // One will be read-only, the other write-only, so we can differentiate when printing.
    void *__capability dc1 = cheri_perms_and(cheri_ddc_get(), CHERI_PERM_STORE);
    void *__capability dc2 = cheri_perms_and(cheri_ddc_get(), CHERI_PERM_LOAD);

    // Create two compartments, and receive the capability that gives us authority to use them
    void *__capability compartment1 = init_compartment(4096, dc1);
    void *__capability compartment2 = init_compartment(4096, dc2);

    // Allocate memory in each compartment
    void *__capability c1_allocated_memory = malloc_compartment(64, compartment1);
    void *__capability c2_allocated_memory = malloc_compartment(256, compartment2);

    // Print an explainer of each allocation (and the capability itself)
    printf("\n\n\tThis capability is heap-allocated within our first compartment, which is "
           "write-only.\n\tWe allocated 64 bytes.\n");
    pp_cap(c1_allocated_memory);

    printf("\n\n\tThis capability is heap-allocated within our second compartment, which is "
           "read-only.\n\tWe allocated 256 bytes.\n");
    pp_cap(c2_allocated_memory);

    // Clean up compartments
    free_compartment(compartment1);
    free_compartment(compartment2);

    printf("\nFreed compartments.\n");

    printf("\nCompleted successfully.\n");
    return 0;
}
