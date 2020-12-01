/**
 * Displays how lib1 and lib2 will look like from the point of view of the users of the libraries.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/lib1.h"
#include "include/lib2.h"

int main()
{
	struct Car *car = new_car();
	car->speed = 1;
	car->honk();

	car->speed = 999;
	car->honk();

	increment();
	increment();
	increment();

	printf("Count: %d\n", get_count());

	increment();
	increment();
	increment();

	printf("Count: %d\n", get_count());
}
