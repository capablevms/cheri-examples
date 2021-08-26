/*
 * Starting from a `read_only` struct, this program
 * restores full privileges using a previous `sealed`
 * capabilty.
 */

#include "../include/common.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>

int main()
{
	// Request special capability `sealcap` from the operating system
	// in order to use it as key to seal `new_small_salary`
	void *__capability sealcap;
	size_t sealcap_size = sizeof(sealcap);
	if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
	{
		error("Fatal error. Cannot get `security.cheri.sealcap`.");
		exit(1);
	}
	assert(cheri_perms_get(sealcap) & CHERI_PERM_SEAL);
	uint8_t *new_small_salary;
	// Seal `new_small_salary` using previously requested `sealcap`
	new_small_salary = (uint8_t *) malloc(sizeof(uint8_t));
	new_small_salary = (uint8_t *) cheri_seal(new_small_salary, sealcap);
	assert(cheri_is_sealed(new_small_salary));
	uint8_t *small_salary;
	small_salary = (uint8_t *) malloc(sizeof(uint8_t));
	assert(cheri_perms_get(small_salary) & (CHERI_PERM_LOAD | CHERI_PERM_STORE));
	pp_cap(small_salary);
	// Make `small_salary` read-only
	small_salary = (uint8_t *) cheri_perms_and(&small_salary, CHERI_PERM_LOAD);
	assert(cheri_perms_get(small_salary) & CHERI_PERM_LOAD);
	pp_cap(small_salary);
	// Restore it to read-write using previously sealed capability `new_small_salary`
	small_salary = (uint8_t *) cheri_unseal(new_small_salary, sealcap);
	assert(cheri_perms_get(small_salary) & (CHERI_PERM_LOAD | CHERI_PERM_STORE));
	pp_cap(small_salary);
}
