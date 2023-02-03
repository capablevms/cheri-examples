/***
 * An example to see what the representable alignment mask is and
 * how it changes the alignment based on how high in memory it is.
 */

#include <cheriintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main()
{
    void *base =
        mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

    uint64_t base_address = cheri_address_get(base);
    for (uint64_t alignment = 0; alignment < (4096 * 10); alignment++)
    {
        uint64_t mask = cheri_representable_alignment_mask(base_address + alignment);
        uint64_t mask1 = cheri_representable_alignment_mask(alignment);
        printf("%lx %lx %lx | %lx %lx %lx\n", mask, base_address + alignment,
               (base_address + alignment) & mask, mask1, alignment, alignment & mask1);
    }
}
