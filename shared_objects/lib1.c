/**
 * A basic example showing how CHERI sealed entries can be used to protect data. In this case the
 * cont variable will be inaccessible from the things that link with this shared library.
 */

#include "include/lib1.h"

static int32_t count = -5;

void increment()
{
	count += 1;
}

int get_count()
{
	if (count > 0)
	{
		return count;
	}
	else
	{
		return 0;
	}
}
