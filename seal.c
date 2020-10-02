#include "include/common.h"
#include <cheriintrin.h>
#include <machine/sysarch.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cheri/cheri.h>
#include <cheri/cheric.h>

/***
 * This is mostly an experiment and it doens't work
 * The CHERI_GET_SEALCAP syscall is mips only
 ***/

typedef struct cap
{
	uint32_t perms : 16;
	uint32_t padding : 4;
	uint32_t otype : 18;
	uint32_t internal_e : 1;
	uint32_t T : 11;
	uint32_t B : 14;
	uint64_t address : 64;
} cap_t;

typedef union
{
	void *ptr;
	cap_t str;
	uint64_t arr[2];
} cheri_pointer;

int main()
{
	void *pcc = cheri_pcc_get();
	inspect_pointer(pcc);

	void *searling_root;
	if (sysarch(CHERI_GET_SEALCAP, &searling_root) < 0)
		searling_root = NULL;

	cheri_pointer data;
	void *first = malloc(64);
	void *other = malloc(64);
	data.ptr = first;
	data.ptr = cheri_tag_clear(data.ptr);
	data.arr[0] = 0;
	data.arr[1] = 0;

	first = cheri_seal(first, other);
	inspect_pointer(first);

	return 0;
	for (uint32_t i = 0; i < 64; i++)
	{
		data.arr[0] |= (1 << i);
		printf("i: %d ", i);
		inspect_pointer(data.ptr);
		data.arr[0] = 0;
	}
}
