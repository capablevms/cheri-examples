#include <stdbool.h>

#include "../../include/common.h"

bool is_pointer(void *ptr)
{
	if (cheri_tag_get(ptr))
	{
		return true;
	}
	return false;
}

bool scan_range(void *ptr, void *exact)
{
	bool found = false;
	for (void *iter = cheri_offset_set(ptr, 0); cheri_offset_get(iter) < cheri_length_get(iter);
		 iter = cheri_offset_set(iter, cheri_offset_get(iter) + 16))
	{
		void *current = *(void **) iter;
		if (is_pointer(current))
		{
			if (cheri_is_sealed(current))
			{
				if (cheri_address_get(current) == cheri_address_get(exact))
				{
					found = true;
					printf("[Exact match] ");
				}
				else if (cheri_length_get(current) == cheri_length_get(exact) &&
						 cheri_base_get(current) == cheri_base_get(exact))
				{
					printf("[Range match] ");
				}

				pp_cap(current);
			}
		}
	}
	return found;
}
