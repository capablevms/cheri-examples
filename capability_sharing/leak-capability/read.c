/*
 * An example which shows how a process can "leak" its capabilities.
 *
 * This example shows how a process can "leak" its capabilities
 *
 * The setup consists of two processes:
 *   * a writer process that writes a capability to a file, and
 *   * a reader process that reads the capability written by the writer process. The
 *     reader processs successfully reads a valid capability
 *
 * This file contains the source of the reader process.
 *
 */
#include <cheriintrin.h>

#include <assert.h>
#include <cheri/cheric.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define CAP_FILE "/tmp/leaked-cap.bin"

/*
 * Read the value of the capability stored in `CAP_FILE`.
 */
int main(int argc, char **argv)
{
    uintptr_t cap = 0;
    // 0 is not a valid capability
    assert(!cheri_is_valid((void *) cap));

    FILE *f = fopen(CAP_FILE, "r");

    if (f == NULL)
    {
        perror("failed to open file");
        return 1;
    }

    size_t n = fread(&cap, sizeof(uintptr_t), 1, f);

    if (n != 1)
    {
        fprintf(stderr, "read failed: %zu\n", n);
        return 1;
    }

    if (fclose(f))
    {
        perror("failed to close file");
        return 1;
    }

    printf("Read capability from %s: %#lp\n", CAP_FILE, (void *) cap);

    // XXX the capability should not be valid but due to a bug in CheriBSD
    // this assertion succeeds!
    assert(cheri_is_valid((void *) cap));

    return 0;
}
