/**
 * Displays how lib1 and lib2 will look like from the point of view of the users of the libraries.
 */

#include <cheri/cheric.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "include/static_variable.h"
#include "include/compartment_per_object.h"
#include "include/unexported_function.h"
#include "include/find_sentries.h"


void static_variables() {
	increment();
	increment();
	increment();

	printf("Count: %d\n", get_count());

	increment();
	increment();
	increment();

	printf("Count: %d\n", get_count());
}

void compartments_per_object() {
	struct Car *car = new_car();
	car->speed = 1;
	car->honk();

	car->speed = 999;
	car->honk();
}

void unexported_functions() {
	printf("Finding do_work using dlsym (main): "); inspect_pointer(dlsym(NULL, "do_work"));
	printf("test: %d\n", test());

	printf(" ------- scanning pcc for sealed pointers ------- \n");
	scan_range(cheri_getpcc());
}

int main()
{
	printf("Choose an exapmle: \n1. Static Variables\n2. OOP Compartments\n3. Unexported Functions\n> ");
	uint32_t example = 0;
	while(scanf("%u", &example) == 1) {
		if(example == 1) {
			static_variables();
		}else if(example == 2) {
			compartments_per_object();			
		}else if(example == 3) {
			unexported_functions();		
		}
		printf("> ");
	}	
}
