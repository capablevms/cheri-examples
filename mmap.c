#include "include/common.h"
#include "include/instructions.h"
#include "include/regs.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>

uint32_t *get_executable_block()
{
	uint32_t *result =
		mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (result == MAP_FAILED)
	{
		printf("ERRNO: %d, ERROR: %s \n\n", errno, strerror(errno));
	}
	return result;
}

uint32_t *generate_purecap(uint32_t *code)
{
	uint32_t idx = 0;

	if (cheri_length_get(code) <= (9 * sizeof(uint32_t)))
	{
		error("Insufficient size");
		exit(-1);
	}

	code[idx++] =
		cincoffsetimm(csp, csp, ((-32) + (2 << 20))); // 0xFE01115B; // cincoffset csp, csp, -32
	code[idx++] = csc_128(csp, cra, 16);			  // 0x00114823; // csc cra, 16(csp)
	code[idx++] = csc_128(csp, cs0, 0);				  // 0x00814023; // csc cs0, 0(csp)
	code[idx++] = cincoffset(cs0, csp, 32);			  // 0x0201145B; // cincoffset cs0, csp, 32
	code[idx++] = addi(a0, zero, 5);				  // 0x00500513; // addi a0, zero, 5
	code[idx++] = clc_128(cs0, csp, 0);				  // 0x0001240F; // clc cs0, 0(csp)
	code[idx++] = clc_128(cra, csp, 16);			  // 0x0101208F; // clc cra, 16(csp)
	code[idx++] = cincoffsetimm(csp, csp, 32);		  // 0x0201115B; // cincoffset csp, csp, 32
	code[idx++] = cjalr(zero, cra);					  // 0xFEC0805B; // cret

	/*
	 * Very important.
	 * The flags of the pointer that is used to call a function are then transferred to the pcc
	 * register. This is important when executing because:
	 * https://github.com/CTSRD-CHERI/sail-cheri-riscv/blob/8253bffe30abf2a8ae1c7eba515061b141aff727/src/cheri_addr_checks.sail#L109
	 * The pcc register is used to determine the mode of operation.
	 * Is it in capability mode or hybrid mode.
	 * When in hybrid mode the capabilities of some registers are ignored.
	 * This means that the ddc (default data capability) is being used.
	 * When in capability mode the capability of the corresponding capability register is used.
	 * For example, if you try to access a0 that would use the capability inside of ca0
	 */
	return cheri_flags_set(code, 0x0001);
}

/*
 * This hybrid function works because the DDC can be set to the stack pointer and used as a
 * reference. This works because when we are in hybrid mode the DDC's address is used as a base. We
 * have the DDC set to be the stack pointer. This allows storing values on the stack though `csc
 * cra, 16(zero)`. The calculateion is done like this: `ddc.address + zero_value + 16`. Therefore
 * cnull can be used in place of the stack pointer in the case of this example.
 */
uint32_t *generate_hybrid(uint32_t *code)
{
	uint32_t idx = 0;

	if (cheri_length_get(code) <= (10 * sizeof(uint32_t)))
	{
		error("Insufficient size");
		exit(-1);
	}

	code[idx++] = cincoffsetimm(csp, csp, ((-32) + (2 << 20)));
	code[idx++] = cspecialrw(cnull, csp, 1); // cspecialw csp, ddc
	code[idx++] = csc_128(zero, cra, 16);
	code[idx++] = csc_128(zero, cs0, 0);
	code[idx++] = cincoffset(cs0, csp, 32);
	code[idx++] = addi(a0, zero, 5);
	code[idx++] = clc_128(cs0, zero, 0);
	code[idx++] = clc_128(cra, zero, 16);
	code[idx++] = cincoffsetimm(csp, csp, 32);
	code[idx++] = cjalr(zero, cra);
	return code;
}

uint32_t *generate_micro(uint32_t *code)
{
	uint32_t idx = 0;

	if (cheri_length_get(code) <= (2 * sizeof(uint32_t)))
	{
		error("Insufficient size");
		exit(-1);
	}
	code[idx++] = addi(a0, zero, 5);
	code[idx++] = cjalr(zero, cra);
	return code;
}

int main()
{
	uint32_t *code;
	code = get_executable_block();
	code = generate_purecap(code);

	inspect_pointer(cheri_pcc_get());
	inspect_pointer(code);

	int (*code_function)() = (int (*)())code;
	int result = code_function();
	printf("Result: %d\n\n", result);
	return result;
}
