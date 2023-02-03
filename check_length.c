/***
 * Testing to see what the representable
 * lengths function returns for different
 * values.
 */

#include <cheriintrin.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
    for (uint64_t length = 0; length <= (4096 * 2); length += 1)
    {
        printf("%lu\n", cheri_representable_length(length));
    }
}
