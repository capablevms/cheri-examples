/**
 * A basic example showing how CHERI sealed entries can be used to protect data. In this case the
 * cont variable will be inaccessible from the things that link with this shared library.
 * Furthermore, each sentry when executed will be the only one in the PCC and so will have no
 * access over its bounds.
 */

#include "include/static_variable.h"
#include "include/find_sentries.h"
#include <assert.h>

static int32_t count = -5;

void increment()
{
	count += 1;
	assert(base_and_bounds_eq(cheri_pcc_get(), &increment));
}

int get_count()
{
	assert(base_and_bounds_eq(cheri_pcc_get(), &get_count));

	if (count > 0)
	{
		return count;
	}
	else
	{
		return 0;
	}
}
