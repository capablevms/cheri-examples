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

typedef struct cheri_object
{
	void *__capability datacap;
	void *__capability codecap;
} CHERI_OBJECT;

CHERI_OBJECT * obj;

// TODO: refactor, move this stuff elsewhere
void print_salary();
void ldpblr(struct cheri_object * pair);
void *__capability seal_immediate(void *__capability cap);

int main()
{
	obj = (CHERI_OBJECT *) malloc(sizeof(CHERI_OBJECT)); 
	uint16_t salary = 1000;
	pp_cap(&print_salary);
	pp_cap(&salary);
	obj->codecap = &print_salary;
	obj->datacap = &salary;
	assert(!cheri_is_sealed(obj->datacap));
	assert(cheri_is_sentry(obj->codecap));
	assert(cheri_is_sealed(obj->codecap));
	obj = (CHERI_OBJECT *) seal_immediate(obj);
	assert(cheri_is_sealed(obj));
	
	ldpblr(obj);

	return 0;
}

inline void ldpblr(struct cheri_object * cheri_obj){
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

void print_salary(){	
	uint16_t * salary_ptr = (obj->datacap);
	printf("Salary: %d\n", *salary_ptr);
}


