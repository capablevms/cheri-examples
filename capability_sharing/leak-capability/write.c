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
 * This file contains the source of the writer process.
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
 * Write a capability to CAP_FILE.
 */
int main(int argc, char **argv)
{
    FILE *f = fopen(CAP_FILE, "w");

    if (f == NULL)
    {
        perror("failed to open file");
        return 1;
    }

    int x = 1;
    uintptr_t cap = (uintptr_t) &x;

    size_t n = fwrite(&cap, sizeof(uintptr_t), 1, f);

    fprintf(stderr, "Wrote capability to %s: %#lp\n", CAP_FILE, (void *) cap);

    if (n != 1)
    {
        fprintf(stderr, "write failed: %zu\n", n);
        return 1;
    }

    if (fclose(f))
    {
        perror("failed to close file");
        return 1;
    }

    return 0;
}
