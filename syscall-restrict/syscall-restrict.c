// Copyright (c) 2022 The CapableVMs "CHERI Examples" Contributors.
// SPDX-License-Identifier: MIT OR Apache-2.0

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <cheriintrin.h>

#include <cheri/cherireg.h>

#define STR(s) #s
#define STR_VALUE(s) STR(s)

typedef int (*stat_fn)(const char *restrict path, struct stat *restrict statbuf);

void try_stat(stat_fn fn, char const *file)
{
    struct stat statbuf;
    printf("    stat(\"%s\", &statbuf)\n", file);
    if (fn(file, &statbuf) == 0)
    {
        // Print some easily-verifiable information to demonstrate that
        // the syscall actually worked.
        printf("    - Success, size is %zu bytes.\n", statbuf.st_size);
    }
    else
    {
        printf("    - Failed, errno = %d: %s\n", errno, strerror(errno));
    }
}

int stat_without_perm_syscall(const char *restrict path, struct stat *restrict statbuf);
#if __has_feature(capabilities) && defined(__aarch64__)
#ifdef __CHERI_PURE_CAPABILITY__
// This probably requires a static build, or at least non-sealed access to the
// underlying 'stat'. Even then, whilst it demonstrates CHERI_PERM_SYSCALL, its
// security is incomplete because the sentry (with the permission) is present in
// every compartment's GOT.
#error Unimplemented: purecap support.
#else
// clang-format off
asm(	".type stat_without_perm_syscall, @function\n"
	"stat_without_perm_syscall:\n"
	// x0: path (passed through unmodified)
	// x1: statbuf (passed through unmodified)
	// First, preserve the current PCC.
	"	adr	x16, stat\n"
	"	cvtp	c16, x16\n"
	// Note that c16 actually refers to the PLT, not to `stat()` itself.
	// That doesn't matter for hybrid mode, since only the PCC address is
	// changed, but it makes this difficult to implement for purecap.
	"	str	c16, [sp, #-32]!\n"
	"	str	lr, [sp, #16]\n"
	// Now drop the permission and call 'stat'. Note that this is not
	// secure; a malicious callee could retrieve the old PCC from the stack.
	// However, it's sufficient for a demonstration.
	"	mov	x17, #" STR_VALUE(CHERI_PERM_SYSCALL) "\n"
	"	clrperm c16, c16, x17\n"
	"	blr	c16\n"
	// Restore registers saved for the call.
	"	ldr	lr, [sp, #16]\n"
	"	ldr	c16, [sp], #32\n"
	// Combine the return address (in lr) with the preserved PCC metadata
	// (in c16) to form a capability that can restore PCC and return at the
	// same time.
	"	scvalue	clr, c16, lr\n"
	"	ret	clr\n");
// clang-format on
#endif
#else
#error This example only supports Morello.
#endif

int main(int argc, char *argv[])
{
    // Arbitrarily pick the executable itself as the subject.
    char const *file = argv[0];

    printf("\nInitially, we can call syscalls as usual:\n");
    try_stat(stat, file);
    printf("\nIf we remove CHERI_PERM_SYSCALL, CheriBSD rejects them:\n");
    try_stat(stat_without_perm_syscall, file);
    printf("\nThe error is conventional (not an exception), so we can recover:\n");
    try_stat(stat, file);
}
