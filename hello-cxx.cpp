#include <iostream>
#include <vector>
#include <cheri.h>
#include "include/common.h"

int main() {
	std::vector<int> list = { 1, 2, 3, 4 };
	for(unsigned int i = 0; i <= 16; i++) {
		inspect_pointer(&list[i]);
		std::cout << "Value: " << list[i] << std::endl;
	}
	
}
