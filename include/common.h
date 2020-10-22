#include <stdio.h>
#include <stdint.h>
#include <cheriintrin.h>
#include <stdbool.h>

void inspect_pointer(void *ptr)
{
	
	uint64_t length = cheri_length_get(ptr);
	uint64_t address = cheri_address_get(ptr);
	uint64_t base = cheri_base_get(ptr);
	uint64_t flags = cheri_flags_get(ptr);
	uint64_t perms = cheri_perms_get(ptr);
	uint64_t type = cheri_type_get(ptr);	
	bool tag = cheri_tag_get(ptr);
	bool valid = cheri_is_valid(ptr);

	uint64_t offset = __builtin_cheri_offset_get(ptr);
	printf("Address: %04lx, Base: %04lx, End: %04lx Flags: %04lx, Length: %04lx, Offset: %04lx, Perms: %04lx, Type: %04lx, Tag: %d, Valid: %d\n",
			address, base, base + length, flags, length, offset, perms, type, tag, valid);
}


void error(char* string) {
	fputs(string, stderr);
	fputc('\n', stderr);
}


// This function returns the current stack pointer.
//
// We don't want to inline it because __builtin_frame_address will change it's meaning.
__attribute__((noinline)) void* cheri_csp_get()
{
	return __builtin_frame_address(0);
}