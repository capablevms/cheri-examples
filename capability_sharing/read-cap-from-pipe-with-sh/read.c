/*
 * An example which fails to "leak" capabilities using Unix pipes.
 *
 * The setup consists of two processes:
 *   * a writer process that writes a capability to stdout, and
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

/*
 * Read the value of a capability from stdin.
 */
int main(int argc, char **argv)
{
    uintptr_t cap = 0;
    // 0 is not a valid capability
    assert(!cheri_is_valid((void *) cap));

    freopen(NULL, "rb", stdin);

    size_t n = fread(&cap, sizeof(uintptr_t), 1, stdin);

    if (n != 1)
    {
        fprintf(stderr, "read failed: %zu\n", n);
        return 1;
    }

    printf("Read capability from stdin: %#lp\n", (void *) cap);

    // As expected, the capability read from stdin is invalid.
    assert(!cheri_is_valid((void *) cap));

    return 0;
}
