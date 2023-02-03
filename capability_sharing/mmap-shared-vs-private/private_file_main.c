#include "util.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    FILE *f = tmpfile();

    if (f == NULL)
    {
        perror("failed to create file");
        return 1;
    }

    int errno;

    // Make sure the file is large enough
    if ((errno = posix_fallocate(fileno(f), 0, MMAP_SIZE)))
    {
        fprintf(stderr, "failed to extend file (error code: %d)\n", errno);
    }

    mmap_test_case_t test = INIT_TEST_CASE(PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(f));

    return run_mmap_test(test);
}
