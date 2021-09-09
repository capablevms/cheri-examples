/* This program  atomically unseal a pair of capabilities
 * (code and data) by calling `ldpblr`, i.e.
 * "Load Pair of capabilities and Branch with Link".
 * See "ARM Architecture Reference Manual Supplement Morello"
 * (https://developer.arm.com/documentation/ddi0606/latest).
 */

#include "../include/common.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>

struct cheri_object
{
	void *__capability datacap;
	void *__capability codecap;
};

void print_salary(uint8_t salary);
void invoke(struct cheri_object * pair);
void *__capability seal_immediate(void *__capability cap);


int main()
{

	// // Request special capability `sealcap` from the operating system
	// // in order to use it as key to seal our capabilities
	// void *__capability sealcap;
	// size_t sealcap_size = sizeof(sealcap);
	// if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
	// {
	// 	error("Fatal error. Cannot get `securiy.cheri.sealcap`.");
	// 	exit(1);
	// }
	// assert(cheri_perms_get(sealcap) & CHERI_PERM_SEAL);
	struct cheri_object * obj = (struct cheri_object *) malloc(sizeof(struct cheri_object)); 
	uint8_t *small_salary = (uint8_t *) malloc(sizeof(uint8_t));
	pp_cap(&print_salary);
	pp_cap(small_salary);

	// Seal the entire struct
	// obj->codecap = &print_salary;
	// obj->datacap = small_salary;
	// // Seal `cheri_object` using previously requested `sealcap`
	// obj = (struct cheri_object *) cheri_seal(obj, sealcap);
	// assert(cheri_is_sealed(obj->codecap));
	// assert(cheri_is_sealed(obj));

	
	obj->datacap = seal_immediate(small_salary);
	obj->codecap = seal_immediate(&print_salary);
	printf("After sealing...\n");
	pp_cap(&print_salary);
	pp_cap(small_salary);
	assert(cheri_is_sealed(obj->datacap));
	assert(cheri_is_sealed(obj->codecap));

	invoke(obj);

	return 0;
}

inline void invoke(struct cheri_object * cheri_obj){
	// Data for the function
	// uint8_t * datacap = (uint8_t *) cheri_obj->datacap;
	// Function pointer (print_salary) of target function
	// void (*codecap)(void) =  cheri_obj->codecap;
	asm(
		"ldpblr c0, [%w[pair]]\n\t"
		"ret\n\t"
		: /* output regs */
		: [pair]"r"(cheri_obj) /* input regs */
  		: /* all caller-saved regs */
	);
	
}

inline void *__capability seal_immediate(void *__capability cap){
	void *__capability sealed_cap;
	asm(
		"seal c1, %w[to_seal], rb\n\t" /* 10 encoded form */
		: [c1] "=r" (sealed_cap)
		: [to_seal] "r" (cap)
		: /* all caller-saved regs */
	);
	return sealed_cap;
}

void print_salary(uint8_t salary){
	printf("Salary: %d", salary);
	fflush(stdout);	
}
