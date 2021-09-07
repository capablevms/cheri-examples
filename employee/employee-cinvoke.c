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
	void *__capability codecap;
	void *__capability datacap;
};

void print_salary(uint8_t salary);
void invoke(struct cheri_object * pair);

int main()
{

	// Request special capability `sealcap` from the operating system
	// in order to use it as key to seal our capabilities
	void *__capability sealcap;
	size_t sealcap_size = sizeof(sealcap);
	if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
	{
		error("Fatal error. Cannot get `securiy.cheri.sealcap`.");
		exit(1);
	}
	assert(cheri_perms_get(sealcap) & CHERI_PERM_SEAL);
	struct cheri_object * obj = (struct cheri_object *) malloc(sizeof(struct cheri_object)); 
	uint8_t *small_salary = (uint8_t *) malloc(sizeof(uint8_t));
	// This should copy the OType of `y` and derive `x` as in `x = cheri_type_copy(x, y)
	// small_salary = cheri_type_copy(small_salary, &print_salary);
	pp_cap(&print_salary);
	pp_cap(small_salary);

	// Seal the entire struct
	// obj->codecap = &print_salary;
	// obj->datacap = small_salary;
	// // Seal `cheri_object` using previously requested `sealcap`
	// obj = (struct cheri_object *) cheri_seal(obj, sealcap);
	// assert(cheri_is_sealed(obj->codecap));
	// assert(cheri_is_sealed(obj));

	// Seal only its parts, i.e. codecap and datacap
	obj->codecap = cheri_seal(&print_salary, sealcap);
	obj->datacap = cheri_seal(small_salary, sealcap);
	assert(cheri_is_sealed(obj->codecap));
	assert(cheri_is_sealed(obj->datacap));

	// FIXME: "Invalid permission for mapped object"
	invoke(obj);

	// assuming the above line works should we be able to
	// print_salary(small_salary)?

	return 0;
}

inline void invoke(struct cheri_object * cheri_obj){
	// Function pointer (print_salary) of target function
	// void (*codecap)(void) =  cheri_obj->codecap;
	// Data for the function
	// uint8_t * datacap = (uint8_t *) cheri_obj->datacap;
	asm(
		"ldpblr c0, [%w[pair]]\n\t"
		: /* output regs */
		: [pair]"r"(cheri_obj) /* input regs */
  		: /* all caller-saved regs */
	);
}

void print_salary(uint8_t salary){
	printf("Salary: %d", salary);
	fflush(stdout);	
}

