/*
 * An example which fails to "leak" capabilities using Unix pipes.
 *
 * The setup consists of two sibling processes connected to a pipe:
 *   * a writer process that writes a capability to the write end of the pipe, and
 *   * a reader process that reads the capability written by the writer process
 *     to the pipe. The reader processs reads an invalid capability
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

#define PIPE_READ 0
#define PIPE_WRITE 1
#define NUM_CHILDREN 2
#define NUM_ITERATIONS 10
#define EXIT_CHILD 0
#define EXIT_PARENT 1

int read_from_pipe(int fds[2])
{
	if (close(fds[PIPE_WRITE]))
	{
		perror("failed to close the write end of the pipe");
		return -1;
	}

	uintptr_t cap = 0;
	// 0 is not a valid capability
	assert(!cheri_is_valid((void *) cap));

	FILE *f = fdopen(fds[PIPE_READ], "r");
	if (f == NULL)
	{
		perror("fdopen fail");
		return -1;
	}

	size_t n = fread(&cap, sizeof(uintptr_t), 1, f);

	if (n != 1)
	{
		fprintf(stderr, "read failed or EOF: %zu\n", n);
		return -1;
	}

	if (fclose(f))
	{
		perror("failed to close stream");
		return -1;
	}

	printf("Read capability from pipe: %#lp\n", (void *) cap);

	// As expected, the capability read from the pipe is invalid.
	assert(!cheri_is_valid((void *) cap));

	return 0;
}

int write_to_pipe(int fds[2])
{
	if (close(fds[PIPE_READ]))
	{
		perror("failed to close the read end of the pipe");
		return -1;
	}

	int x = 1;
	uintptr_t cap = (uintptr_t) &x;

	fprintf(stderr, "Writing capability to pipe: %#lp\n", (void *) cap);

	FILE *f = fdopen(fds[PIPE_WRITE], "w");
	if (f == NULL)
	{
		perror("fdopen fail");
		return -1;
	}

	size_t n = fwrite(&cap, sizeof(uintptr_t), 1, f);

	if (n != 1)
	{
		fprintf(stderr, "write failed or EOF: %zu\n", n);
		return 1;
	}

	if (fclose(f))
	{
		perror("failed to close stream");
		return 1;
	}

	return 0;
}

int spawn_child(int (*child_fn)(int[2]), int fds[2])
{
	pid_t pid = fork();
	switch (pid)
	{
	case -1:
	{
		perror("failed to spawn child process");
		return -1;
	}
	case 0:
	{
		if (child_fn(fds))
		{
			return -1;
		}
		return 0;
	}
	default:
	{
		break;
	}
	}

	// Parent
	return pid;
}

/*
 * Fork two child processes - one that writes to the pipe (`write_to_pipe`), and
 * another that reads from the pipe (`read_from_pipe`)
 */
int run_pipe_test()
{
	int fds[2];

	if (pipe(fds))
	{
		perror("failed to create pipe");
		return -1;
	}

	// The writer process:
	int res = spawn_child(write_to_pipe, fds);

	// Only continue if we're the parent and no error occurred.
	if (res == EXIT_CHILD || res == -1)
	{
		return res;
	}

	// The reader process:
	res = spawn_child(read_from_pipe, fds);

	// Only continue if we're the parent and no error occurred.
	if (res == EXIT_CHILD || res == -1)
	{
		return res;
	}

	return EXIT_PARENT;
}

/*
 * Write a capability to a pipe.
 */
int main(int argc, char **argv)
{
	for (size_t i = 0; i < NUM_ITERATIONS; ++i)
	{
		switch (run_pipe_test())
		{
		case -1:
		{
			fprintf(stderr, "failed after %zu iterations\n", i);
			return -1;
		}
		case EXIT_CHILD:
		{
			return 0;
		}
		default:
		{
			// We're the parent process (continue to the next iteration).
			continue;
		}
		}
	}

	for (size_t i = 0; i < NUM_CHILDREN * NUM_ITERATIONS; ++i)
	{
		if (wait(NULL) == -1 && errno != ECHILD)
		{
			perror("failed to wait for child");
			return 1;
		}
	}

	return 0;
}
