//#include <machine/cheric.h>
//#include <cheri.h>
//#include <cheriintrin.h>
#include <cheri.h>
#include <cheri/cheric.h>
//#include <cheri/libcheri_fd.h>
//#include <cheri/libcheri_sandbox.h>
#include <machine/sysarch.h>
#include <stdio.h>
#include <assert.h>
//#include "../include/common.h"
#include "include/utils.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#define SANDBOX_CODE_SZ  1000  // approx. later modify it.

/// commented out cheriintrin.h ////
typedef long cheri_otype_t;
#define cheri_is_sealed(x) __builtin_cheri_sealed_get(x)
/// from cheriintrin.h          ////

//extern 
//void* __capability 
//cheri_codeptrperm (void (*codeptr)(void), ptraddr_t len, long perm);

//extern 
//void* __capability 
//cheri_ptrperm(void * ptr, size_t len, long perm); 

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

void sandbox_1_func()
{
    /*
    struct cheri_object sb1_obj;
    sb1_obj.co_codecap = sandbox_1_codecap;
	sb1_obj.co_datacap = sandbox_1_datacap;

    (void)libcheri_invoke(sb1_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    */
    printf("--> end_of_sandbox_1_func\n");
}

void sandbox_2_func()
{
    /*
    struct cheri_object sb2_obj;
    
    sb2_obj.co_codecap = sandbox_2_codecap;
	sb2_obj.co_datacap = sandbox_2_datacap;

    (void)libcheri_invoke(sb2_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    */ 
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
         //error("Fatal error. Cannot get `security.cheri.sealcap`.");
         printf("Fatal error. Cannot get `security.cheri.sealcap`.");
         exit(1);
    }
    
    void *__capability sealcap2;
    size_t sealcap_size2 = sizeof(sealcap2);
    if (sysctlbyname("security.cheri.sealcap", &sealcap2, &sealcap_size2, NULL, 0) < 0)
    {
         //error("Fatal error. Cannot get `security.cheri.sealcap`.");
         printf("Fatal error. Cannot get `security.cheri.sealcap`.");
         exit(1);
    }

    // otype 0~3 is reserved on morello.
    // 4 ~ 2^13 is available for userspace
    //- sandbox_1 sealcap -// 
    cheri_otype_t oty_1 = cheri_maketype(sealcap, sealcap_size); 
    cheri_otype_t oty_2 = cheri_maketype(sealcap2, sealcap_size2); 
                            // -> cheri_maketype return type is otype??

    printf("sealcap_oty_1: %lu, sealcap_oty_2: %lu\n", oty_1, oty_2);
    assert(CHERI_OTYPE_USER_MIN <= oty_1 && oty_1 <= CHERI_OTYPE_USER_MAX);
    assert(CHERI_OTYPE_USER_MIN <= oty_2 && oty_2 <= CHERI_OTYPE_USER_MAX);

    //- sandbox_1 code cap -// 
    void * sandbox_code_1_ubound = (void*)(((size_t)sandbox_1_func) + SANDBOX_CODE_SZ); 
        // TODO (1) test with overlapped address range of objcap with diff permissoins
        //      (2) test with preciser addr bounds 
    sandbox_1_codecap = cheri_seal(codecap_create((void (*)(void))&sandbox_1_func, sandbox_code_1_ubound), 
                                   //sandbox_1_sealcap);
                                   sealcap);
   
    //- sandbox1's data's range is overlapped with sandbox2's -//
    size_t sandbox_data_1_lbound = (size_t)apprx_data_UB - 1000; // approx
    size_t sandbox_data_1_ubound = (size_t)apprx_data_UB;
    sandbox_1_datacap = cheri_seal(datacap_create((void*)sandbox_data_1_lbound, (void*)sandbox_data_1_ubound), 
                                   //sandbox_1_sealcap);
                                   sealcap);
   

    //-- Setup sandbox_2 object capability --//
    
    //- sandbox_2 sealcap -// 
    //sandbox_2_sealcap = (void *__capability)cheri_maketype(sealcap, sealcap_size); 
    
    //- sandbox_2 code cap -// 
    void* sandbox_2_code_base = (void*)(&sandbox_2_func);
    void* sandbox_2_code_end = (void*)(((size_t)sandbox_2_code_base) + SANDBOX_CODE_SZ); 
    sandbox_2_codecap = cheri_seal(codecap_create((void(*)(void))(&sandbox_2_func), sandbox_2_code_end), 
                                   //sandbox_2_sealcap);
                                   sealcap2);
    
    //- sandbox_2 data cap -// 
    void * sandbox2_lbound = (void*)((size_t)apprx_data_UB - 1000); // approx 
    sandbox_2_datacap = cheri_seal(datacap_create(sandbox2_lbound, apprx_data_UB), 
                                   //sandbox_2_sealcap);
                                   sealcap2);

    printf("sb_1_code_otype: %lu\n", cheri_type_get(sandbox_1_codecap));
    printf("sb_2_code_otype: %lu\n", cheri_type_get(sandbox_1_codecap));
    //assert(cheri_type_get(sandbox_1_sealcap) != cheri_type_get(sandbox_2_sealcap));

    // just check 
    assert(cheri_is_sealed(sandbox_1_codecap));
    assert(cheri_is_sealed(sandbox_1_datacap));
	 
    // print
    printf("\n---sandbox_1_codecap ---------\n");
    //pp_cap((void* __capability)sandbox_1_codecap);
    printf("\n---sandbox_1_datacap ---------\n");
    //pp_cap((void* __capability)sandbox_1_datacap);
    
    sandbox_1_func();
    sandbox_2_func();
    
    //- Setup a capability of a heap object first -//
   
    /*
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
	
    */

    return 0;
}

