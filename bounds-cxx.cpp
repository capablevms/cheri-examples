#include "include/common.h"
#include <cheri.h>
#include <iostream>
#include <vector>

int main()
{
	std::vector<int> list = {1, 2, 3, 4};
	for (unsigned int i = 0; i <= 16; i++)
	{
		pp_cap(&list[i]);
		std::cout << "Value: " << list[i] << std::endl;
	}
}
