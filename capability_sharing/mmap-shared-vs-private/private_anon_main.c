#include "util.h"

#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    mmap_test_case_t test = INIT_TEST_CASE(PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1);

    return run_mmap_test(test);
}
