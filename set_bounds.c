/***
 * The point of this program is to demonstrate the failing of a program
 * that tries to extend the bounderies of a capable pointer.
 * Compared to the simpler 'bounds.c', here we try to modify the bounds
 * explicitly.
 ***/

#include "include/common.h"
#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int32_t array[16] = {0};
    uint32_t bounds = 64;

    if (argc < 2)
    {
        puts("Bounds [Choose a value greater than 64]:");
        if (0 == scanf("%u", &bounds))
        {
            error("Extraneous input");
        }
    }
    // Command line argument to simplify testing
    else if (atoi(argv[1]) > 64)
    {
        bounds = atoi(argv[1]);
    }
    else
    {
        printf("Please choose a value greater than 64.");
        // This will cause the test to fail for a value lower than 64
        exit(0);
    }
    // Trying to extend the bounds generates an exception.
    int32_t *custom_bounds_array = cheri_setbounds(array, bounds);
}
