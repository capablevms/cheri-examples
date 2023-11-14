#include "compartment_alloc.h"
#include "utils.h"
#include <cheriintrin.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

heap_compartment compartments[maxCompartments];
int numCompartments = 0;

/*
 * Initialise a compartment with `size_in_bytes` space, where all future
 * allocations will be constrained by `dc` as if it was the DDC when allocation
 * occurred.
 */
void *__capability init_compartment(size_t size_in_bytes, void *__capability dc)
{
    void *buf;
    void *__capability compartment_id;

    if (numCompartments + 1 == maxCompartments)
    {
        perror("Too many compartments requested.");
        exit(1);
    }

    // Allocate memory for this compartment
    buf = mmap(NULL, size_in_bytes, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (buf == MAP_FAILED)
    {
        perror("error in initial mem allocation");
        exit(-1);
    }

    // Create the new compartment
    compartment_id = sealed_reference(buf);
    compartments[numCompartments] = (heap_compartment){
        buf,           // Space on the heap
        0,             // bytes allocated (currently none!)
        size_in_bytes, // Our maximum size, to check when allocating
        dc,            // The data capability to allocate against (like a DDC)
        compartment_id // An identifier which allows a holder to allocate in this component
    };
    numCompartments++;

    // Return the sealed capability which acts as a token of authentication for a component.
    return compartment_id;
}

/*
 * Allocate `len` bytes in the compartment identified by `compartment_id`.
 * if `compartment_id` doesn't exist, returns NULL
 */
void *__capability malloc_compartment(size_t len, void *__capability compartment_id)
{
    size_t rounded_length;
    size_t new_allocated;
    heap_compartment compartment = compartments[0];
    void *__capability allocated;
    void *addr;
    int i;

    // Search for a compartment with the given identifier
    for (i = 0; i < maxCompartments && compartments[i].identifier != compartment_id; i++)
    {
    }
    if (i == maxCompartments)
    {
        perror("Given an ID for a non-existent compartment");
        exit(1);
    }

    compartment = compartments[i];

    // Try to "bump-allocate" some space in the compartment's buffer
    addr = compartment.buffer + compartment.bytes_allocated;

    addr = __builtin_align_up(addr, ~cheri_representable_alignment_mask(len) + 1);
    rounded_length = cheri_representable_length(len);

    new_allocated = (addr + rounded_length) - compartment.buffer;
    if (new_allocated > compartment.max_allocated)
    {
        perror("Maximum # bytes in compartment exceeded.");
        exit(1);
    }

    compartment.bytes_allocated = new_allocated;

    // We allocated some space!
    // Create a capability pointing to it and return that.
    // The capability inherits the metadata of data capability for this compartment.
    allocated = cheri_address_set(compartment.datacap, (long) addr);
    allocated = cheri_bounds_set_exact(allocated, rounded_length);

    return allocated;
}

/*
 * Unmaps the compartment identified by the `compartment_id` argument,
 * effectively freeing it.
 *
 * Note that this is a naive implementation for demo purposes. The capabilities
 * previously returned by `malloc_compartment` now point to de-allocated memory.
 */
void free_compartment(void *__capability compartment_id)
{
    int munmap_rc;
    int i;

    // Search for a compartment with the given identifier
    for (i = 0; i < maxCompartments && compartments[i].identifier != compartment_id; i++)
    {
    }
    if (i == maxCompartments)
    {
        perror("Given an ID for a non-existent compartment");
        exit(1);
    }

    munmap_rc = munmap(compartments[i].buffer, compartments[i].max_allocated);
    if (munmap_rc != 0)
    {
        perror("Attempted to deallocate a compartment, but munmap errored");
        exit(1);
    }
}
