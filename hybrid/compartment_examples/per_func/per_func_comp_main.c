//#include <cheri/cheri.h>
//#include <cheri/cheric.h>
//#include <cheri/libcheri_fd.h>
//#include <cheri/libcheri_sandbox.h>

#include <stdio.h>
#include <assert.h>
#include "../include/common_orig.h"
#include "include/utils.h"
#include <stdlib.h>
#include <sys/types.h>

#define SANDBOX_CODE_SZ  1000  // approx. later modify it.

extern 
void* __capability 
cheri_codeptrperm (void (*codeptr)(void), ptraddr_t len, long perm);

extern 
void* __capability 
cheri_ptrperm(void * ptr, size_t len, long perm); 

static void * __capability sandbox_1_sealcap;
static void * __capability sandbox_1_codecap;
static void * __capability sandbox_1_datacap;

static void * __capability sandbox_2_sealcap;
static void * __capability sandbox_2_codecap;
static void * __capability sandbox_2_datacap;
    
static void * __capability
codecap_create(void (*sandbox_base)(void), void *sandbox_end)
{
	void * __capability codecap;

#ifdef __CHERI_PURE_CAPABILITY__
	(void)sandbox_end;
	codecap = cheri_andperm(sandbox_base,
	    CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_EXECUTE);
#else
	codecap = cheri_codeptrperm(sandbox_base,
	    (size_t)sandbox_end - (size_t)sandbox_base,
	    CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_EXECUTE); 
#endif
	return (codecap);
}

// ?? CHERI_PERM_INVOKE is not used??

static void * __capability
datacap_create(void *sandbox_base, void *sandbox_end)
{
	void * __capability datacap;

#ifdef __CHERI_PURE_CAPABILITY__
	(void)sandbox_end;
	datacap = cheri_andperm(sandbox_base,
	    CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_STORE |
	    CHERI_PERM_LOAD_CAP | CHERI_PERM_STORE_CAP |
	    CHERI_PERM_STORE_LOCAL_CAP);
#else
	datacap = cheri_ptrperm(sandbox_base, // TODO: define sandbox_base/end
	    (size_t)sandbox_end - (size_t)sandbox_base,
	    CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_STORE |
	    CHERI_PERM_LOAD_CAP | CHERI_PERM_STORE_CAP |
	    CHERI_PERM_STORE_LOCAL_CAP);
#endif
	return (datacap);
}

/*
//
// TODO: test function attributes (ccall, ccallee), with args
//
__attribute__((cheri_ccall))
__attribute__((cheri_method_suffix("_cap")))
__attribute__((cheri_method_class(main_obj)))
int sandbox_1_func(int * __capability intcap);

int sandbox_1_func(int * __capability intcap)
{
    struct cheri_object sb1_obj;
    
    sb1_obj.co_codecap = sandbox_1_codecap;
	sb1_obj.co_datacap = sandbox_1_datacap;

    (void)libcheri_invoke(sb1_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return *intcap + 1;
}

__attribute__((cheri_ccall))
__attribute__((cheri_method_suffix("_cap")))
__attribute__((cheri_method_class(sandbox2_obj)))
int sandbox_2_func(int * __capability intcap);

int sandbox_2_func(int * __capability intcap)
{
    struct cheri_object sb2_obj;
    
    sb2_obj.co_codecap = sandbox_2_codecap;
	sb2_obj.co_datacap = sandbox_2_datacap;

    (void)libcheri_invoke(sb2_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return *intcap + 1; 
}
*/

int sandbox_1_func(int * __capability intcap)
{
    struct cheri_object sb1_obj;
    
    sb1_obj.co_codecap = sandbox_1_codecap;
	sb1_obj.co_datacap = sandbox_1_datacap;

    (void)libcheri_invoke(sb1_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    printf("--> end_of_sandbox_2_func\n");
}

void sandbox_2_func()
{
    struct cheri_object sb2_obj;
    
    sb2_obj.co_codecap = sandbox_2_codecap;
	sb2_obj.co_datacap = sandbox_2_datacap;

    (void)libcheri_invoke(sb2_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    printf("--> end_of_sandbox_2_func\n");
}

/*
__attribute__((cheri_ccall))
__attribute__((cheri_method_suffix("_cap")))
__attribute__((cheri_method_class(main_obj)))
*/

int main () // remove unused args
{
    //-- Setup sandbox_1 object capability --//
    
    void * apprx_data_UB = __builtin_frame_address(0);
    
    //- Get a sealing capability 
    void *__capability sealcap;
    size_t sealcap_size = sizeof(sealcap);
    if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
    {
         error("Fatal error. Cannot get `security.cheri.sealcap`.");
         exit(1);
    }
    
    //- sandbox_1 sealcap -// 
    //sandbox_1_sealcap = (void *__capability)cheri_maketype(sealcap, tysz); 
    sandbox_1_sealcap = (void *__capability)cheri_maketype(sealcap, sealcap_size); 
                        // cheri_maketype return type -> ?? 
    
    //- sandbox_1 code cap -// 
    void * sandbox_code_1_ubound = (void*)(((size_t)sandbox_1_func) + SANDBOX_CODE_SZ); 
        // TODO (1) test with overlapped address range of objcap with diff permissoins
        //      (2) test with preciser addr bounds 
    sandbox_1_codecap = cheri_seal(codecap_create((void (*)(void))&sandbox_1_func, sandbox_code_1_ubound), 
                                   sandbox_1_sealcap);
   
    //- sandbox1's data's range is heap lower bound  ~ upper bound -//
    size_t sandbox_data_1_lbound = apprx_data_UB - 1000; // approx
    size_t sandbox_data_1_ubound = apprx_data_UB;
    sandbox_1_datacap = cheri_seal(datacap_create((void*)sandbox_data_1_lbound, (void*)sandbox_data_1_ubound), 
                                   sandbox_1_sealcap);

    //-- Setup sandbox_2 object capability --//
    
    //- sandbox_2 sealcap -// 
    sandbox_2_sealcap = (void *__capability)cheri_maketype(sealcap, sealcap_size); 
    
    //- sandbox_2 code cap -// 
    void* sandbox_2_code_base = (void*)(&sandbox_2_func);
    void* sandbox_2_code_end = (void*)(((size_t)sandbox_2_code_base) + SANDBOX_CODE_SZ); 
    sandbox_2_codecap = cheri_seal(codecap_create((void(*)(void))(&sandbox_2_func), sandbox_2_code_end), 
                                   sandbox_2_sealcap);
    
    //- sandbox_2 data cap -// 
    void * sandbox2_lbound = (void*)((size_t)apprx_data_UB - 1000); // approx 
    sandbox_2_datacap = cheri_seal(datacap_create(sandbox2_lbound, apprx_data_UB), 
                                   sandbox_2_sealcap);

    // just check 
    assert(cheri_is_sealed(sandbox_1_codecap));
    assert(cheri_is_sealed(sandbox_1_datacap));
    
    // print
    printf("\n---sandbox_1_codecap ---------\n");
    pp_cap((void* __capability)sandbox_1_codecap);
    printf("\n---sandbox_1_datacap ---------\n");
    pp_cap((void* __capability)sandbox_1_datacap);
    
    *heap_cap = sandbox_1_func(heap_cap);
    printf("\n --- heapcap_4/4 ---------\n");
    printf("heapval_internal: %d (11?)\n", *heap_cap);
    
    *heap_cap = sandbox_2_func(heap_cap);
    printf("\n -- ERROR ----- heapval_external: %d (ERROR: this should not be printed)\n", *heap_cap);
    
    //- Setup a capability of a heap object first -//
    
    int * heap_ptr = (int *)malloc (sizeof(int));
    int *__capability heap_cap = (__cheri_tocap int *__capability)heap_ptr;
    write_ddc((void *__capability) heap_cap); 
   
    heap_cap = cheri_address_set((void *__capability)heap_cap, (unsigned long)heap_ptr);
    heap_cap = cheri_bounds_set((void *__capability)heap_cap, sizeof(int));
    cheri_perms_and(heap_cap, CHERI_PERM_LOAD | CHERI_PERM_STORE | CHERI_PERM_LOAD_CAP);
    
    printf("\n --- heapcap_2/4 ---------\n");
    pp_cap((void *__capability) heap_cap);
    
    *heap_cap = 10;
    
    //- sandbox1's data's range is heap lower bound  ~ upper bound -//
    size_t sandbox_data_1_lbound = cheri_base_get(heap_cap);
    size_t sandbox_data_1_ubound = sandbox_data_1_lbound + cheri_length_get(heap_cap) - 1;
    sandbox_1_datacap = cheri_seal(datacap_create((void*)sandbox_data_1_lbound, (void*)sandbox_data_1_ubound), 
                                   sandbox_1_sealcap);
    
    /*
    
    main_obj.co_datacap = (__cheri_tocap void *__capability)cheri_ddc_get();
    void * stack_addr = __builtin_frame_address(0);
    main_obj.co_datacap = cheri_address_set((void *__capability)main_obj.co_datacap, 
		    			    (unsigned long)stack_addr); // todo: check val 
    main_obj.co_datacap = cheri_perms_and(main_obj.co_datacap, CHERI_PERM_STORE | CHERI_PERM_LOAD | CHERI_PERM_LOAD_CAP | CHERI_PERM_SEAL);
    main_obj.co_datacap = cheri_seal(main_obj.co_datacap, sealcap);
    
    int * heap_ptr = (int *)malloc (sizeof(int));
    int *__capability heap_cap = (__cheri_tocap int *__capability)heap_ptr;
    write_ddc((void *__capability) heap_cap); 
    printf("\n --- heapcap_1/4 ---------\n");
    pp_cap((void *__capability) heap_cap);
   
    heap_cap = cheri_address_set((void *__capability)heap_cap, (unsigned long)heap_ptr);
    heap_cap = cheri_bounds_set((void *__capability)heap_cap, sizeof(int));
    cheri_perms_and(heap_cap, CHERI_PERM_LOAD | CHERI_PERM_STORE | CHERI_PERM_LOAD_CAP);
    
    printf("\n --- heapcap_2/4 ---------\n");
    pp_cap((void *__capability) heap_cap);
    
    *heap_cap = 10;
    printf("\n --- heapcap_3/4 ---------\n");
    printf("*heap_cap: %d\n", *heap_cap);

    *heap_cap = sandbox_1_func(heap_cap);
    printf("\n --- heapcap_4/4 ---------\n");
    printf("heapval_internal: %d (11?)\n", *heap_cap);
    
    *heap_cap = sandbox_2_func(heap_cap);
    printf("\n -- ERROR ----- heapval_external: %d (ERROR: this should not be printed)\n", *heap_cap);
    */

	return 0;
}

