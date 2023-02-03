/*
 * According to the CHERI C/C++ programming guide, "in the CheriABI process
 * environment, only untagged data (not tagged pointers) may be written to or
 * read from files. If a region of memory containing valid pointers is written
 * to a file, and then read back, the pointers in that region will no longer be
 * valid.". However, as this example demonstrates, this behaviour appears to be
 * broken, as it is possible to read a valid capability from a file.
 */
#include <cheriintrin.h>

#include <assert.h>
#include <cheri/cheric.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

int write_cap(FILE *f, uintptr_t *cap)
{
    size_t res = fwrite(cap, sizeof(uintptr_t), 1, f);
    if (res != 1)
    {
        fprintf(stderr, "write failed: %zu\n", res);
        return 1;
    }

    return 0;
}

int read_cap(FILE *f, uintptr_t *cap)
{
    size_t res = fread(cap, sizeof(uintptr_t), 1, f);
    if (res != 1)
    {
        fprintf(stderr, "read failed: %zu\n", res);
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int x = 1;
    uintptr_t cap = (uintptr_t) &x;
    FILE *f = NULL;

    if (!(f = tmpfile()))
    {
        perror("failed to create temporary file");
        return 1;
    }

    printf("Writing capability to file: %#lp\n", (void *) cap);
    assert(cheri_is_valid((void *) cap));

    if (write_cap(f, &cap))
    {
        return 1;
    }

    if (fflush(f))
    {
        perror("failed to flush");
        return 1;
    }

    if (fseek(f, 0, SEEK_SET))
    {
        perror("failed to seek");
        return 1;
    }

    uintptr_t stored_cap = 0;
    // 0 is not a valid capability
    assert(!cheri_is_valid((void *) stored_cap));

    if (read_cap(f, &stored_cap))
    {
        return 1;
    }

    printf("Read capability from file: %#lp\n", (void *) stored_cap);
    assert(cap == stored_cap);

    // XXX the capability should not be valid but due to a bug in CheriBSD
    // this assertion succeeds!
    assert(cheri_is_valid((void *) stored_cap));

    fclose(f);

    return 0;
}
