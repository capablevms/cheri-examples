/**
 * Check whether the PCC covers only one function
 * in shared objects (static_variable, static_function)
 */
#include "include/find_sentries.h"
#include "include/static_function.h"
#include "include/static_variable.h"

int main()
{
	increment();
	check_static_and_non_static();
	return 0;
}