#include "include/common.h"

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

int main()
{

	jmp_buf buffer;
	int res = setjmp(buffer);
	//printf("setjmp return: %d\n", res);

	uint32_t length = cheri_length_get(buffer);
	//inspect_pointer(buffer);
	//printf("len: %d\n", length);


	// buffer[0] == _JB_MAGIC_SETJMP == 0xbe87fd8a2910af01
	// buffer[1] == $csp
	// buffer[2] == $cfp
	// buffer[3..13] == $cs1..11
	// buffer[14..31] = ???
	for (uint32_t idx; idx < (length / 16); idx++)
	{
		if (cheri_is_valid(((void **)buffer)[idx]))
		{
			void* csp = cheri_csp_get();
			uint64_t base = cheri_base_get(csp);
			uint64_t length = cheri_length_get(csp);
			uint64_t address = cheri_address_get(((void**)buffer)[idx]);

			if(address >= base && address <= (base + length)) {
				printf("[STACK POINTER] ");
			}
			inspect_pointer(((void **)buffer)[idx]);
		}
	}
}
