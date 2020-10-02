#include "include/common.h"

#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *csp;
void *stack_top;

void inspect_stack(void *stack)
{
	printf("offset: %lu\n", cheri_length_get(stack) - cheri_offset_get(stack));
}

bool is_stack_pointer(void *ptr)
{
	if (cheri_is_valid(ptr))
	{
		uint64_t base = cheri_base_get(stack_top);
		uint64_t length = cheri_length_get(stack_top);
		uint64_t address = cheri_address_get(ptr);

		if (address >= base && address <= (base + length))
		{
			return true;
		}
	}
	return false;
}

bool is_pointer(void* ptr)
{
	if (cheri_is_valid(ptr)) {
		return true;
	}
	return false;
}

bool is_exec(void* ptr) 
{
	if(((cheri_perms_get(ptr) & 0b10) >> 1) == 1) {
		return true;
	}
	return false;
}

void scan_range(void* start, void* end) {
	
	void** location = (void**)start;
	//printf("Scanning: ");
	//inspect_pointer(start);
	//inspect_pointer(end);
	
	while(location > end) {
		if(is_pointer(*location)) {
			if(is_stack_pointer(*location)) {
				printf("[Stack Pointer] ");
			}
			if(is_exec(*location)) {
				printf("[Executable] ");
			}
			inspect_pointer(*location);
		}
		location -= 1;
	}
}

void scan_frames()
{
	// We dont's scan the part of the stack taken by the current function
	// I promise the collect function doesn't allocate
	void *stack_bot = __builtin_frame_address(0);
	void *previous_frame = *(void **)stack_bot;

	inspect_stack(stack_bot);
	inspect_stack(previous_frame);
	while (previous_frame < stack_top)
	{
		void **next_frame = (void **)previous_frame;
		if(!is_stack_pointer(*next_frame)) {
			//printf("Couldn't find a stack pointer at: %p, looking down the stack", next_frame);
		}
		while (!is_stack_pointer(*next_frame))
		{
			printf("doesn't contain a stack pointer: ");
			inspect_stack(next_frame);
			next_frame -= 1;
		}

		scan_range(*next_frame, previous_frame);
		previous_frame = *next_frame;
		printf(" ======= "); inspect_stack(previous_frame);
	}
}

void scan_all() {
	void *stack_bot = __builtin_frame_address(0);
	scan_range(stack_top, stack_bot);
}

int test_2()
{
	//void *cs0;
	//asm("csc cs0, %0"	: "=rm"(cs0));
	//asm("csc csp, %0"		: "=rm"(csp));
	//inspect_stack(cs0);
	//inspect_stack(csp);
	scan_all();
	return 99;
}

int test_3()
{
	return test_2();
}

uint32_t* test()
{
	uint32_t* values = (uint32_t*)malloc((size_t)test_3());
	values[0] = 42;
	values[1] = 42;
	return values;
}

int main()
{
	stack_top = __builtin_frame_address(0);

	//inspect_stack(stack_top);
	uint64_t base = cheri_base_get(csp);
	uint64_t length = cheri_length_get(csp);

	uint32_t* values = test();
	
	for(int idx = 0; idx <= 1; idx++) {
		printf("Value: %d\n", values[idx]);
	}

	scan_all();
	return 0;
}
