/**
 * A more complicated example of sealed entries displaying how they can be used to protect private
 * data that will and can be given to the user of the library as a pointer. In this case Car_priv is
 * the private fields and methods of the object that is given to the user, and it is protected by
 * exploiting the properties of the sealed entries (The fact that the capability can only be jumped
 * to not modified in any way).
 */

#include <cheri/cheri.h>
#include <cheri/cheric.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>

#include "../include/instructions.h"
#include "../include/regs.h"
#include "include/compartment_per_object.h"

void crash()
{
	printf("CRASH!\n");
}

void honk(struct Car_priv *priv, struct Car *car)
{
	printf("%p %p\n", priv, car);
	printf("HONK!\n");
	if (car->speed > priv->maxSpeed)
	{
		priv->crash();
	}
}

struct Car *new_car()
{
	uint32_t *mem =
		mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

	void **ptrs = mem;
	
	const uint32_t vtable_start_index = 2;
	ptrs[vtable_start_index + 0] = honk;

	const ptrdiff_t vtable_offset = ((char*)(ptrs + vtable_start_index) - (char*)ptrs);
	const size_t vtable_size = 1 * 16;

	uint32_t idx = 0;
	mem[idx++] = auipcc(cs2, 0);
	mem[idx++] = clc_128(cs3, cs2, vtable_offset);
	mem[idx++] = cincoffsetimm(ca0, cs2, vtable_offset + vtable_size);
	mem[idx++] = cincoffsetimm(ca1, cs2, vtable_offset + vtable_size + sizeof(struct Car_priv));
	mem[idx++] = cjalr(cnull, cs3);

	uint32_t functions_size = vtable_offset + vtable_size;
	struct Car *public_car =
		(struct Car *)(((char *)mem) + sizeof(struct Car_priv) + functions_size);
	struct Car_priv *private_car = (struct Car_priv *)(((char *)mem) + functions_size);
	
	private_car->maxSpeed = 10;
	private_car->crash = &crash;

	public_car->honk = cheri_sealentry(cheri_setflags(mem, 1));

	return public_car;
}
