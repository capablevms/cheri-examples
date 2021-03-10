#include <cheri/cheri.h>
#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>

#include <stdbool.h>

void inspect_pointer(void *ptr)
{
	uint64_t length = cheri_getlength(ptr);
	uint64_t address = cheri_getaddress(ptr);
	uint64_t base = cheri_getbase(ptr);
	uint64_t flags = cheri_getflags(ptr);
	uint64_t perms = cheri_getperm(ptr);
	uint64_t type = cheri_gettype(ptr);
	bool tag = cheri_gettag(ptr);

	uint64_t offset = cheri_getoffset(ptr);
	printf("Address: %04lx, Base: %04lx, End: %04lx Flags: %04lx, Length: %04lx, Offset: %04lx, "
		   "Perms: %04lx, Type: %04lx, Tag: %d\n",
		   address, base, base + length, flags, length, offset, perms, type, tag);
}

void error(char *string)
{
	fputs(string, stderr);
	fputc('\n', stderr);
}

// This function returns the current stack pointer.
//
// We don't want to inline it because __builtin_frame_address will change it's meaning.
// __builtin_frame_address gives you the stack bottom of the current function.
// This will be the stack top of the calling function.
__attribute__((noinline)) void *cheri_getcsp()
{
	return __builtin_frame_address(0);
}
