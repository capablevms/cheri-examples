cmake_minimum_required (VERSION 3.15.0) 

project(CHERI_EXAMPLES 
	VERSION 0.1 
	DESCRIPTION "Stub examples showing how to exploit capabilities"
	LANGUAGES C)

get_filename_component(COMP_NAME ${CMAKE_C_COMPILER} NAME_WE)

add_executable(allocate allocate.c) 
add_executable(check_length check_length.c) 
add_executable(check_mask check_mask.c) 
add_executable(function function.c) 
add_executable(hello hello.c) 
add_executable(mmap mmap.c) 
if( ${COMP_NAME} MATCHES "^mips*" )
  add_executable(seal seal.c) 
endif() 
add_executable(sentry sentry.c) 
add_executable(set_bounds set_bounds.c) 
add_executable(setjmp setjmp.c) 
add_executable(stackscan stackscan.c) 
add_executable(xor_pointers xor_pointers.c)
add_executable(bitmap_alloc_good example_allocators/bitmap_allocator/good_client.c
                                 example_allocators/bitmap_allocator/bitmap_alloc.c)
