/**
 * Displays how lib1 and lib2 will look like from the point of view of the users of the libraries.
 */

#include <assert.h>
#include <cheri/cheric.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/compartment_per_object.h"
#include "include/find_sentries.h"
#include "include/static_variable.h"
#include "include/unexported_function.h"

void static_variables()
{
	increment();
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
	increment();
	increment();

	printf("Count: %d\n", get_count());

	increment();
	increment();
	increment();

	printf("Count: %d\n", get_count());
}

void compartments_per_object()
{
	struct Car *car = new_car();
	car->speed = 1;
	car->honk();

	car->speed = 999;
	car->honk();
}

void unexported_functions()
{
	printf("Finding do_work using dlsym (main): ");
	pp_cap(dlsym(NULL, "do_work"));
	void *function_to_search_for = test();
	printf("test: %p\n", function_to_search_for);

	printf(" ------- scanning pcc for sealed pointers ------- \n");
	bool found = scan_range(cheri_getpcc(), function_to_search_for);
	assert(found == false);
}

int main()
{
	printf("Choose an exapmle: \n1. Static Variables\n2. OOP Compartments\n3. Unexported "
		   "Functions\n> ");
	uint32_t example = 0;
	while (scanf("%u", &example) == 1)
	{
		if (example == 1)
		{
			static_variables();
		}
		else if (example == 2)
		{
			compartments_per_object();
		}
		else if (example == 3)
		{
			unexported_functions();
		}
		printf("> ");
	}
}
