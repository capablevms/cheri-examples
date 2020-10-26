#include "include/common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b)
{
	if (0 == a)
	{
		return b;
	}
	else if (0 == b)
	{
		return a;
	}
	else if (a == b)
	{
		return a;
	}
	else if (a > b)
	{
		return gcd(a - b, b);
	}
	return gcd(a, b - a);
}

int main()
{

	int a, b = 0;

	printf("Enter first number:");
	if (0 == scanf("%d", &a))
	{
		error("Extraneous input");
		return -1;
	}

	printf("Enter second number:");
	if (0 == scanf("%d", &b))
	{
		error("Extraneous input");
		return -1;
	}

	int c_gcd = gcd(a, b);
	printf("The gcd of these numbers is: %d\n", c_gcd);
	inspect_pointer(*gcd);

	return 0;
}
