#include "common.h"
#include "regs.h"
#include "instructions.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

uint32_t* code; 

uint32_t *get_executable_block()
{
	uint32_t* result = mmap(
		NULL,
		512,
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_ANON | MAP_PRIVATE,
		-1,
		0);
	
	if(result == MAP_FAILED) {
		printf("ERRNO: %d, ERROR: %s \n\n", errno, strerror(errno));
	}
	return result;

}

void generate(uint32_t* code) {
	uint32_t idx = 0;
	code[idx++] = cincoffsetimm(csp, csp, ((-32) + (2 << 20))); //0xFE01115B; // cincoffset csp, csp, -32
	code[idx++] = csc_128(csp, cra, 16); //0x00114823; // csc cra, 16(csp)
	code[idx++] = csc_128(csp, cs0, 0); //0x00814023; // csc cs0, 0(csp)
	code[idx++] = cincoffset(cs0, csp, 32); //0x0201145B; // cincoffset cs0, csp, 32
	code[idx++] = addi(a0, zero, 5); //0x00500513; // addi a0, zero, 5
	code[idx++] = clc_128(cs0, csp, 0); //0x0001240F; // clc cs0, 0(csp)
	code[idx++] = clc_128(cra, csp, 16); //0x0101208F; // clc cra, 16(csp)
	code[idx++] = cincoffsetimm(csp, csp, 32); //0x0201115B; // cincoffset csp, csp, 32
	code[idx++] = cjalr(zero, cra); //0xFEC0805B; // cret
}

int main()
{
	code = get_executable_block();
	generate(code);



	// Very important, the flags of the pointer that is used to call a function are then transfered to the pcc register 
	// The flags in the pcc reigster matter when executing: https://github.com/CTSRD-CHERI/sail-cheri-riscv/blob/8253bffe30abf2a8ae1c7eba515061b141aff727/src/cheri_addr_checks.sail#L109
	// The registyer is used to determine the mode of operation. Is it in capability mode or in hybrid mode.
	// When in hybrid mode the capabilities of some registers don't matter and the ddc (default data capability) is being used.
	// When in capability mode the capability of the coresponding capability register is used, if you try to access a0 that would use the capability inside of ca0
	//
	code = cheri_flags_set(code, 0x0001);
	inspect_pointer(cheri_pcc_get());
	inspect_pointer(code);


	//	code[0] = 0x00500513; // addi a0, zero, 5
	//	code[1] = 0xFEC0805B; // cret

	int (*code_function)() = (int (*)())code;
	int result = ((int (*)())code)();
	printf("Result: %d\n\n", result);
	return result;
}
