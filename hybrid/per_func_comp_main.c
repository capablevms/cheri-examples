//#include <machine/cheric.h>
//#include <cheri.h>
//#include <cheriintrin.h>
//#include <cheri/libcheri_fd.h>
//#include <cheri/libcheri_sandbox.h>
#include <cheri.h>
#include <cheri/cheri.h>
#include <cheri/cheric.h>
#include <machine/sysarch.h>
#include <stdio.h>
#include <assert.h>
//#include "../include/common.h"
#include "include/utils.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#define SANDBOX_CODE_SZ  1000  // approx. later modify it.

/// cheriintrin.h is commented out ////
typedef long cheri_otype_t;
#define cheri_is_sealed(x) __builtin_cheri_sealed_get(x)
/// from cheriintrin.h  ////

//extern 
//void* __capability 
//cheri_codeptrperm (void (*codeptr)(void), ptraddr_t len, long perm);

//extern 
//void* __capability 
//cheri_ptrperm(void * ptr, size_t len, long perm); 

//static void * __capability sandbox_1_sealcap;
static void * __capability cheriobj_1_codecap;
static void * __capability cheriobj_1_datacap;

//static void * __capability sandbox_2_sealcap;
static void * __capability cheriobj_2_codecap;
static void * __capability cheriobj_2_datacap;

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
    
    sb1_obj.co_codecap = cheriobj_1_codecap;
	sb1_obj.co_datacap = cheriobj_1_datacap;

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
    
    sb2_obj.co_codecap = cheriobj_2_codecap;
	sb2_obj.co_datacap = cheriobj_2_datacap;

    (void)libcheri_invoke(sb2_obj, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return *intcap + 1; 
}
*/

void sandbox_1_func()
{
    printf("--> end_of_sandbox_1_func\n");
}

void sandbox_2_func()
{
    printf("--> end_of_sandbox_2_func\n");
}


// TODO: try with func_attr 
/*
__attribute__((cheri_ccall))
__attribute__((cheri_method_suffix("_cap")))
__attribute__((cheri_method_class(main_obj)))
*/


int main () // Remove unused args
{
    //-- Setup sandbox_1 object capability --//
    
    //- Get a sealing capability 
    
    void *__capability sealcap;
    size_t sealcap_size = sizeof(sealcap);
    if (sysctlbyname("security.cheri.sealcap", &sealcap, &sealcap_size, NULL, 0) < 0)
    {
         printf("Fatal error. Cannot get `security.cheri.sealcap`.");
         exit(1);
    }
        // TODO: try with cheri_otype_alloc
        // otype_t cheri_otype_alloc(void);
    
        // otype 0~3 is reserved on morello.
        // 4 ~ 2^13 is available for userspace
        // cheri_otype_t oty = cheri_maketype(sealing_cap, type); 
        //                        // ?? -> cheri_maketype return type is otype??
    
    //- Create cheri_object_1 sealcap -// 
    void * __capability sealcap_1 = cheri_maketype(sealcap, sealcap_size); 
    
    //- Create cheri_object_1 code cap -// 
    void * sandbox_code_1_ubound = (void*)(((size_t)sandbox_1_func) + SANDBOX_CODE_SZ); 
        // TODO (1) test with overlapped address range of objcap with diff permissoins
        //      (2) test with precise addr bounds later 
    cheriobj_1_codecap = cheri_seal(codecap_create((void (*)(void))&sandbox_1_func, sandbox_code_1_ubound), 
                                   sealcap_1);
   
    //- Create cheri_object_1 data cap -// 
    
        //- Two cheri_objs's data addr ranges are overlapped -//
    void * apprx_data_UB = __builtin_frame_address(0);
    
    size_t cheriobj_data_1_base = (size_t)apprx_data_UB - 1000; // approx
    size_t cheriobj_data_1_end = (size_t)apprx_data_UB;
    cheriobj_1_datacap = cheri_seal(datacap_create((void*)cheriobj_data_1_base, (void*)cheriobj_data_1_end), 
                                   sealcap_1);
    
    //-- Setup cheri_object_2 --//
    
    //- Create cheri_object_2 sealcap -// 
    void * __capability sealcap_2 = cheri_maketype(sealcap, sealcap_size + sizeof(sealcap)); 
        // TODO: increase offset --> this is so stupid. Fix it later
    
    //- Create cheri_object_2 code cap -// 
    void* cheriobj_2_code_base = (void*)(&sandbox_2_func);
    void* cheriobj_2_code_end = (void*)(((size_t)cheriobj_2_code_base) + SANDBOX_CODE_SZ); 
    cheriobj_2_codecap = cheri_seal(codecap_create((void(*)(void))(&sandbox_2_func), cheriobj_2_code_end), 
                                   sealcap_2);
    
    assert((size_t)&sandbox_1_func != (size_t)&sandbox_2_func);

    //- Create cheri_object_2 data cap -// 
    void * cheriobj_2_data_base = (void*)((size_t)apprx_data_UB - 1000); // approx 
    cheriobj_2_datacap = cheri_seal(datacap_create(cheriobj_2_data_base, apprx_data_UB), 
                                   sealcap_2);

    // Just check 
    assert(cheri_is_sealed(cheriobj_1_codecap));
    assert(cheri_is_sealed(cheriobj_1_datacap));
    assert(cheri_is_sealed(cheriobj_2_codecap));
    assert(cheri_is_sealed(cheriobj_2_datacap));
    
    printf("___cheri_object__otypes____\n");
    printf("code_1_otype: %u\n", cheri_type_get(cheriobj_1_codecap));
    printf("data_1_otype: %u\n", cheri_type_get(cheriobj_1_datacap));
    printf("code_2_otype: %u\n", cheri_type_get(cheriobj_2_codecap));
    printf("data_2_otype: %u\n", cheri_type_get(cheriobj_2_datacap));
    
    printf("___cheri_object__perms____\n");
    printf("code_1_perms: %u\n", cheri_perms_get(cheriobj_1_codecap));
    printf("data_1_perms: %u\n", cheri_perms_get(cheriobj_1_datacap));
    printf("code_2_perms: %u\n", cheri_perms_get(cheriobj_2_codecap));
    printf("data_2_perms: %u\n", cheri_perms_get(cheriobj_2_datacap));
  
        // User-defined cheri_object's otype should be within valid range  
    assert(CHERI_OTYPE_USER_MIN <= cheri_type_get(cheriobj_1_codecap) \
            && cheri_type_get(cheriobj_1_codecap) <= CHERI_OTYPE_USER_MAX);
    assert(CHERI_OTYPE_USER_MIN <= cheri_type_get(cheriobj_2_codecap) \
            && cheri_type_get(cheriobj_1_codecap) <= CHERI_OTYPE_USER_MAX);
   
        // Cheri_object_1 and 2's otypes should differ
    assert(cheri_type_get(cheriobj_1_codecap) != cheri_type_get(cheriobj_2_codecap));
    assert(cheri_type_get(cheriobj_1_datacap) != cheri_type_get(cheriobj_2_datacap));
    
        // Each cheri_object's code and data address ranges are same,
        // but perms for code and data differ.
        // TODO: Test obj cap invoke 
    assert(cheri_perms_get(cheriobj_1_codecap) != cheri_perms_get(cheriobj_1_datacap));
    assert(cheri_perms_get(cheriobj_2_codecap) != cheri_perms_get(cheriobj_2_datacap));
	 
    sandbox_1_func();
    sandbox_2_func();
    
    return 0;
}

