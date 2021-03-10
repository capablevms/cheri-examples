#include <cheri/cheric.h>
#include <stdbool.h>

#include "../../include/common.h"

bool is_pointer(void *ptr)
{
	if (cheri_gettag(ptr))
	{
		return true;
	}
	return false;
}

bool scan_range(void *ptr, void *exact)
{
	bool found = false;
	for (void *iter = cheri_setoffset(ptr, 0); cheri_getoffset(iter) < cheri_getlength(iter);
		 iter = cheri_incoffset(iter, 16))
	{
		void *current = *(void **)iter;
		if (is_pointer(current))
		{
			if (cheri_getsealed(current))
			{
				if (cheri_getaddress(current) == cheri_getaddress(exact))
				{
					found = true;
					printf("[Exact match] ");
				}
				else if (cheri_gettop(current) == cheri_gettop(exact) &&
						 cheri_getbase(current) == cheri_getbase(exact))
				{
					printf("[Range match] ");
				}

				inspect_pointer(current);
			}
		}
	}
	return found;
}
