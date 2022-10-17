/*
 * An example which fails to "leak" capabilities using Unix pipes.
 *
 * The setup consists of two processes spawned by the shell:
 * * a writer process (whose stdout is redirected to a pipe by the shell), which
 *   writes a capability to stdout
 * * a reader process which reads the capability from stdin (its stdin is redirected
 *   to the same pipe the writer process writes to). The reader processs reads an
 *   invalid capability
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

/*
 * Write a capability to stdout.
 */
int main(int argc, char **argv)
{
	freopen(NULL, "wb", stdout);

	int x = 1;
	uintptr_t cap = (uintptr_t) &x;

	size_t n = fwrite(&cap, sizeof(uintptr_t), 1, stdout);

	fprintf(stderr, "Wrote capability to stdout: %#lp\n", (void *) cap);

	if (n != 1)
	{
		fprintf(stderr, "write failed: %zu\n", n);
		return 1;
	}

	return 0;
}
