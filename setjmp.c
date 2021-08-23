#include "include/common.h"
#include <cheri/cheric.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
	jmp_buf buffer;
	int res = setjmp(buffer);

	uint32_t length = cheri_getlength(buffer);

	// buffer[0] == _JB_MAGIC_SETJMP == 0xbe87fd8a2910af01
	// buffer[1] == $csp
	// buffer[2] == $cfp
	// buffer[3..13] == $cs1..11
	// buffer[14..31] = ???
	for (uint32_t idx; idx < (length / 16); idx++)
	{
		if (cheri_gettag(((void **)buffer)[idx]))
		{
			void *csp = cheri_getcsp();
			uint64_t address = cheri_getaddress(((void **)buffer)[idx]);

			if (cheri_is_address_inbounds(csp, address))
			{
				printf("[STACK POINTER] ");
			}
			pp_cap(((void **)buffer)[idx]);
		}
	}
}
