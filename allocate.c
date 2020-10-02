#include "include/common.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	size_t size = 31;
	printf("Size: ");
	scanf("%lu", &size);
	void* memory = malloc(size);
	inspect_pointer(memory);

}
