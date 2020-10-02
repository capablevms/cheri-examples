#include<stdio.h>
#include<stdint.h>
#include <cheriintrin.h>
#include<stdbool.h>

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

// This should be a macro or inlined
// In the current case it gives the stack pointer
// inside of the cheri_csp_get function.
// This is fine if only the capabilitits are important.
// In any other case the csp will need to be decremented
// to get the actual csp of the calling function.
void* cheri_csp_get()
{
	void *value;
	asm("csc csp, %0"	: "=rm"(value));
	return value;
}

