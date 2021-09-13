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
void print_something();
void invoke(struct cheri_object * pair);
void *__capability seal_immediate(void *__capability cap);


int main()
{
	struct cheri_object * obj = (struct cheri_object *) malloc(sizeof(struct cheri_object)); 
	uint8_t *small_salary = (uint8_t *) malloc(sizeof(uint8_t));
	pp_cap(&print_salary);
	pp_cap(small_salary);
	obj->codecap = &print_something;
	*small_salary = 12;
	obj->datacap = small_salary;
	assert(!cheri_is_sealed(obj->datacap));
	// obj->datacap = seal_immediate(small_salary);
	assert(cheri_is_sentry(obj->codecap));
	assert(cheri_is_sealed(obj->codecap));
	// obj->codecap = seal_immediate(&print_salary);
	invoke(obj);
	printf("Just in case, this is small_salary: %d\n", *small_salary);

	// qemu system crash:
	// cheri_utils.h:225 "Should only be used with sentry cap"
	// ~/cheri/morello-qemu/target/cheri-common/cheri_utils.h
	// A sentry is already sealed (by definition!) and every subsequent call to 
	// sealing functions will clear the tag!!!
	

	return 0;
}

inline void invoke(struct cheri_object * cheri_obj){
	// Data for the function
	// uint8_t * datacap = (uint8_t *) cheri_obj->datacap;
	// Function pointer (print_salary) of target function
	// void (*codecap)(void) =  cheri_obj->codecap;
	asm(
		"ldpblr c0, [%w[pair]]\n\t"
		: /* output regs */
		: [pair]"r"(cheri_obj) /* input regs */
  		: "c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","c10","c11","c12","c13","c14","c15","c16","c17","c18","clr","d8","d9","d10","d11","d12","d13","d14","d15"
		  /* clobbered registers */
	);
	
}

inline void *__capability seal_immediate(void *__capability cap){
	void *__capability sealed_cap;
	asm(
		"seal %[sealed_cap], %[cap], rb\n\t"
		: [sealed_cap] "=C" (sealed_cap)
		: [cap] "C" (cap)
		: 
	);
	return sealed_cap;
}

void print_salary(uint8_t salary){
	printf("Salary: %d\n", salary);
	fflush(stdout);
}

void print_something(){
	printf("Hello, I have been unsealed!?\n");
}

