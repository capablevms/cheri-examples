#include "include/static_function.h"
#include "include/find_sentries.h"
#include <assert.h>
#include <stdio.h>

static int static_function()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &static_function));
	return 0;
}

static double another_static_function()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &another_static_function));
	return 1.0;
}

void non_static_fun()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &non_static_fun));
	assert(base_and_bounds_eq(cheri_pcc_get(), &static_function));
	printf("Called static function with result: %d\n", static_function());
}

void non_static_fun2()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &non_static_fun2));
	assert(base_and_bounds_eq(cheri_pcc_get(), &another_static_function));
	printf("Called another function with result: %f\n", another_static_function());
}

void check_static_and_non_static()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &non_static_fun));
	assert(base_and_bounds_eq(cheri_pcc_get(), &static_function));
	assert(base_and_bounds_eq(cheri_pcc_get(), &non_static_fun2));
	assert(base_and_bounds_eq(cheri_pcc_get(), &another_static_function));
	assert(base_and_bounds_eq(cheri_pcc_get(), &check_static_and_non_static));
}
