/*
    This example implements a simple cheri type allocator 
    to show how cheri otype values are determined.
    
    Note: cheriintrin.h is not used in this example.

*/

#include <assert.h>
#include <cheri.h>
#include <cheri/cheri.h>
#include <cheri/cheric.h>
#include <machine/sysarch.h>
#include <stdio.h>
#include "../include/print.h"
#include "include/utils.h"
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/types.h>

#define SANDBOX_CODE_SZ 1000 // approx. later modify it.

//- NOTE ------  
/// cheriintrin.h is commented out temporarily in this example //
typedef long cheri_otype_t;
#define cheri_is_sealed(x)      __builtin_cheri_sealed_get(x)
// from cheriintrin.h  ///

// static void * __capability sandbox_1_sealcap;
static void *__capability cheriobj_1_codecap;
static void *__capability cheriobj_1_datacap;

// static void * __capability sandbox_2_sealcap;
static void *__capability cheriobj_2_codecap;
static void *__capability cheriobj_2_datacap;

static void *__capability codecap_create(void (*sandbox_base)(void), void *sandbox_end)
{
    void *__capability codecap;

#ifdef __CHERI_PURE_CAPABILITY__
    (void) sandbox_end;
    codecap = cheri_andperm(sandbox_base, CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_EXECUTE);
#else
    codecap = cheri_codeptrperm(sandbox_base, (size_t) sandbox_end - (size_t) sandbox_base,
                                CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_EXECUTE);
#endif
    return (codecap);
}

// ?? CHERI_PERM_INVOKE is not used??

static void *__capability datacap_create(void *sandbox_base, void *sandbox_end)
{
    void *__capability datacap;

#ifdef __CHERI_PURE_CAPABILITY__
    (void) sandbox_end;
    datacap = cheri_andperm(sandbox_base, CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_STORE |
                                            CHERI_PERM_LOAD_CAP | CHERI_PERM_STORE_CAP |
                                            CHERI_PERM_STORE_LOCAL_CAP);
#else
    datacap =
        cheri_ptrperm(sandbox_base, // TODO: define sandbox_base/end
                      (size_t) sandbox_end - (size_t) sandbox_base,
                      CHERI_PERM_GLOBAL | CHERI_PERM_LOAD | CHERI_PERM_STORE | CHERI_PERM_LOAD_CAP |
                          CHERI_PERM_STORE_CAP | CHERI_PERM_STORE_LOCAL_CAP);
#endif
    return (datacap);
}

//- Create a global_sealcap to track the next cheri_otype -// 
void *__capability global_sealcap;

//- Simple type allocator - Takes an otype size; returns the next otype -// 
void * __capability alloc_type (size_t tysz)
{
    // TODO: Try with cheri_otype_alloc :: otype_t cheri_otype_alloc(void);
    // cheri_otype 0~3      : reserved by morello, 
    //             4~ 2^13  : available for userspace
    // Check how Cheri makes a type: "cheri_otype_t cheri_maketype(sealing_cap, type)"
    //          ?? -> cheri_maketype return type is otype??
    
    void * __capability sealcap;
    sealcap = cheri_maketype(global_sealcap, tysz); 
    __builtin_cheri_cap_type_copy(global_sealcap, sealcap);
    return sealcap;

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
///
/// up_to_here
/// 
*/

void sandbox_1_func()
{
    printf("--> end_of_sandbox_1_func\n");
}

void sandbox_2_func()
{
    printf("--> end_of_sandbox_2_func\n");
}


int main() // Remove unused args
{
    printf("----------------------------------------\n");
    printf("CHERI_OTYPE_USER_MIN: %04x\n", CHERI_OTYPE_USER_MIN);
    printf("CHERI_OTYPE_USER_MAX: %04x\n", CHERI_OTYPE_USER_MAX);
    printf("----------------------------------------\n\n");
    
    //=- Init a global_sealing capability -=//
    
    size_t global_sealcap_sz = sizeof(global_sealcap); //sizeof(global_sealcap);
    if (sysctlbyname("security.cheri.sealcap", &global_sealcap, &global_sealcap_sz, NULL, 0) < 0)
    {
        printf("Fatal error. Cannot get `security.cheri.sealcap`.");
        exit(1);
    }
    
    //=- Create cheri_object_1 sealcap -=//
    
    //- Get a next type i.e. create a next sealing capability -//
    size_t sealcap_size_1 = 12; // --> Random size is given to test how types are assigned 
    void * __capability sealcap_1 = alloc_type(sealcap_size_1);

    //- NOTE: Since this example is just to check otypes, 
    //- we set arbitrary address ranges of code and data capability

    //- Create cheri_object_1 *code* cap -//
    
    void *sandbox_code_1_ubound = (void *) (((size_t) sandbox_1_func) + SANDBOX_CODE_SZ);
    cheriobj_1_codecap = cheri_seal(codecap_create((void (*)(void)) & sandbox_1_func, sandbox_code_1_ubound), 
                                    sealcap_1);
    
    //- Create cheri_object_1 *data* cap -//

    //- Two cheri_objects' data addr ranges are set overlapped. Ignore the numbers for this example.  -//
    void *apprx_data_UB = __builtin_frame_address(0);

    size_t cheriobj_data_1_base = (size_t) apprx_data_UB - 1000; // approx
    size_t cheriobj_data_1_end = (size_t) apprx_data_UB;
    cheriobj_1_datacap = cheri_seal(datacap_create((void *) cheriobj_data_1_base, (void *) cheriobj_data_1_end), 
                                    sealcap_1);
    
    //=- Setup cheri_object_2 -=//

    //- Create cheri_object_2 sealcap -//
    
    size_t sealcap_size_2 = 32; // arbitrary type size for sealcap2
    void * __capability sealcap_2 = alloc_type(sealcap_size_2);

    //- Create cheri_object_2 *code* cap -//
    
    void *cheriobj_2_code_base = (void *) (&sandbox_2_func);
    void *cheriobj_2_code_end = (void *) (((size_t) cheriobj_2_code_base) + SANDBOX_CODE_SZ);
    cheriobj_2_codecap = cheri_seal(codecap_create((void (*)(void))(&sandbox_2_func), cheriobj_2_code_end), 
                                    sealcap_2);

    assert((size_t) &sandbox_1_func != (size_t) &sandbox_2_func);

    //- Create cheri_object_2 *data* cap -//
    void *cheriobj_2_data_base = (void *) ((size_t) apprx_data_UB - 1000); // approx
    cheriobj_2_datacap = cheri_seal(datacap_create(cheriobj_2_data_base, apprx_data_UB), 
                                    sealcap_2);

    // Check
    assert(cheri_is_sealed(cheriobj_1_codecap));
    assert(cheri_is_sealed(cheriobj_1_datacap));
    assert(cheri_is_sealed(cheriobj_2_codecap));
    assert(cheri_is_sealed(cheriobj_2_datacap));
    
    printf("____global_sealcap__________\n");
    print_cap(global_sealcap);

    printf(">>>> cheri_object_1__________________________\n");
    printf("Sealcap_1_user_defined_tysz : %04lx\n", sealcap_size_1);
    printf("___sealcap_1_info____\n");
    print_cap(sealcap_1);
    printf("___code_cap_1_sealed_info____\n");
    print_cap(cheriobj_1_codecap);
    printf("___data_cap_1_sealed_info____\n");
    print_cap(cheriobj_1_datacap);
    
    printf(">>>> cheri_object_2__________________________\n");
    
    printf("Sealcap_2_user_defined_tysz : %04lx\n", sealcap_size_2);
    printf("___sealcap_2_info____\n");
    print_cap(sealcap_2);
    printf("___code_cap_2_sealed_info____\n");
    print_cap(cheriobj_2_codecap);
    printf("___data_cap_2_sealed_info____\n");
    print_cap(cheriobj_2_datacap);
    
    printf(" ** code_1_type: %04x\n", cheri_type_get(cheriobj_1_codecap));
    printf(" ** data_1_type: %04x\n", cheri_type_get(cheriobj_1_datacap));
    printf(" ** code_2_type: %04x\n", cheri_type_get(cheriobj_2_codecap));
    printf(" ** data_2_type: %04x\n\n", cheri_type_get(cheriobj_2_datacap));

    // User-defined cheri_object's otype should be within valid range
    assert(CHERI_OTYPE_USER_MIN <= cheri_type_get(cheriobj_1_codecap) &&
           cheri_type_get(cheriobj_1_codecap) <= CHERI_OTYPE_USER_MAX);
    assert(CHERI_OTYPE_USER_MIN <= cheri_type_get(cheriobj_2_codecap) &&
           cheri_type_get(cheriobj_2_codecap) <= CHERI_OTYPE_USER_MAX);
    
    printf(" ** Success: all code/data capability types are within valid range ** \n");
    
    // Cheri_object_1 and 2's otypes should differ
    assert(cheri_type_get(cheriobj_1_codecap) != cheri_type_get(cheriobj_2_codecap));
    assert(cheri_type_get(cheriobj_1_datacap) != cheri_type_get(cheriobj_2_datacap));
    
    printf(" ** Success: code and data capability types differ ** \n");

    // Each cheri_object's code and data address ranges are set the same,
    // but perms for code and data differ.
    // TODO: Test obj cap invoke
    assert(cheri_perms_get(cheriobj_1_codecap) != cheri_perms_get(cheriobj_1_datacap));
    assert(cheri_perms_get(cheriobj_2_codecap) != cheri_perms_get(cheriobj_2_datacap));
    
    printf(" ** Success: code and data capability perms differ ** \n");

    // TODO: cheri_invoke
    sandbox_1_func();
    sandbox_2_func();

    return 0;
}
