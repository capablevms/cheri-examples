/***
 * This program shows that putting "NULL" into the DDC causes an
 * "In-address space security exception"
 ***/

#include "../include/common.h"
#include "include/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

int main()
{
    // Ensure the DDC contains a valid capability
    assert(cheri_tag_get(cheri_ddc_get()));
    // Putting a NULL will cause the exception
    write_ddc(NULL);
    return 0;
}