/* This program  atomically unseal a pair of capabilities
 * (code and data) by calling `ldpblr`, i.e.
 * "Load Pair of capabilities and Branch with Link".
 * It shows that `ldpblr` can unseal the pair but the unsealed
 * result is not copied to any intermediate register. Thus,
 * it is not available outside the call. This program, in fact,
 * crashes when tries to read `datacap`.
 * 
 * See "ARM Architecture Reference Manual Supplement Morello"
 * (https://developer.arm.com/documentation/ddi0606/latest)
 * for further details regarding `ldpblr`.
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

void print_salary();
void ldpblr(struct cheri_object * pair);
CHERI_OBJECT * seal_immediate_pair(CHERI_OBJECT * obj);

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
	obj = seal_immediate_pair(obj);
	assert(cheri_is_sealed(obj));
	
	ldpblr(obj);

	return 0;
}

inline void ldpblr(CHERI_OBJECT * obj){
	asm(
		"ldpblr c0, [%w[obj]]\n\t"
		: /* output regs */
		: [obj]"r"(obj) /* input regs */
  		: "c0","c1","c2","c3","c4","c5","c6","c7","c8","c9","c10","c11","c12","c13","c14","c15","c16","c17","c18","clr","d8","d9","d10","d11","d12","d13","d14","d15"
		  /* clobbered registers */
	);
}

inline CHERI_OBJECT * seal_immediate_pair(CHERI_OBJECT * obj){
	CHERI_OBJECT * sealed_obj;
	asm(
		"seal %[sealed_obj], %[obj], lpb\n\t"
		: [sealed_obj] "=C" (sealed_obj)
		: [obj] "C" (obj)
		: 
	);
	return sealed_obj;
}

void print_salary(){	
	// Since `obj` is sealed, the following line
	// causes an `In-address security exception`
	uint16_t * salary_ptr = (obj->datacap);
	printf("Salary: %d\n", *salary_ptr);
}




