#include <stdbool.h>
#include <cheri/cheric.h>

#include "../../include/common.h"

bool is_pointer(void *ptr)
{
	if (cheri_gettag(ptr))
	{
		return true;
	}
	return false;
}

void scan_range(void * ptr)
{
	for(void* iter = cheri_setoffset(ptr, 0); cheri_getoffset(iter) < cheri_getlength(iter); iter = cheri_incoffset(iter, 16))
	{
		if (is_pointer(*(void**)iter))
		{
			if(cheri_getsealed(*(void**)iter)) {
				inspect_pointer(*(void**)iter);
			}
		}
	}
}

