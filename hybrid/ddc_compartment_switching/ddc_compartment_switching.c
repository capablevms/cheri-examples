/***
 * TODO: something like intermediate level of compartment switching or ...
 ***/

#include "../../include/common.h"
#include "../include/utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


#if !defined(__CHERI_CAPABILITY_WIDTH__) || defined(__CHERI_PURE_CAPABILITY__)
#error "This example only works on CHERI hybrid mode"
#endif

// Simple function with some data which will be part of the compartment
int compartment_simple_fun(int a, int b);
extern int switch_compartment(int a, int b, void * stack, size_t size);

int main(){
    void * simple_block = malloc(5000);
    size_t compartment_size = 2000;
    switch_compartment(5, 3, simple_block, compartment_size);
    return 0;
}

int compartment_simple_fun(int a, int b) {
   return a + b;
}
