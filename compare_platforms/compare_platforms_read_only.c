/** 
 * This version can be run in both a CHERI and a non-CHERI environment to 
 * compare the two.
 * 
 * In a CHERI environment, it will:
 *     Step 1: Use can_read to check if the struct's "permissions" member includes write permissions.
 *     Step 2: Use capabilities to enforce read-only permissions
 *             (see https://github.com/capablevms/cheri-examples/blob/master/employee/include/employee.h).
 *     Step 3: Try to write to the read-only struct members, and SIGPROT.
 *
 * In a non-CHERI environment, it will:
 *     Step 1: Use can_read to check if the struct's "permissions" member includes write permissions.
 *     Step 2: Try to exceed the bounds of an object and add write permissions
 *             (see https://ctsrd-cheri.github.io/cheri-exercises/exercises/buffer-overflow-heap/index.html).
 *     Step 3: Rewrite the review.
 *
 * In a CHERI environment, the overflow attack will be unreachable.
 * In morello-purecap, it would crash immediately whether or not the struct
 * had been set as read only.
 *
 * In a non-CHERI environment, this attack should succeed in corrupting some of the data.
 *
 */

#include "include/review.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *username, *realname, *privatereview, *publicreview, *permissions;
    
    // Our new review, which will have read only permissions.
    struct review review;
    
    size_t smallsz = 0x20;
    size_t biggersz = 0x1001;
    
    // Populate the members of the review struct.
    username = malloc(smallsz);
    strcpy(username, "Reviewer 2");    
    size_t szn = actual_input_length(username, smallsz);
    
    // Reviewer's real name
    realname = malloc(smallsz);
    strcpy(realname, "Baba Yaga");
    size_t szp = actual_input_length(realname, smallsz);
    
    // The reviewer's private comments
    privatereview = malloc(biggersz);
    strcpy(privatereview, "I cannot believe I read this appalling piece of dreck from start to finish. The authors should be ashamed, and I hope I get an opportunity to tell them so to their faces.");
    size_t szprr = actual_input_length(privatereview, biggersz);

    // The public review
    publicreview = malloc(biggersz);
    strcpy(publicreview, "I am a little unclear as to the contribution. I think the authors could strengthen their case considerably if they conducted an RCT. Weak reject.");
    size_t szpur = actual_input_length(publicreview, biggersz);
    
    // The permissions
    permissions = malloc(smallsz);
    strcpy(permissions, "r");
    size_t szpm = actual_input_length(permissions, smallsz);
    
    review.username = username;
    review.realname = realname;
    review.privatereview = privatereview;
    review.publicreview = publicreview;
    review.permissions = permissions;
    print_details(&review);

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nsmallsz=%zx, CRRL(smallsz)=%zx\n", smallsz,
        __builtin_cheri_round_representable_length(smallsz));
    printf("biggersz=%zx, CRRL(biggersz)=%zx\n", biggersz,
        __builtin_cheri_round_representable_length(biggersz));
    printf("username=%#p realname=%#p diff=%tx\n", username, realname, realname - username);
    printf("realname=%#p privatereview=%#p diff=%tx\n", realname, privatereview, privatereview - realname);
    printf("privatereview=%#p publicreview=%#p diff=%tx\n", privatereview, publicreview, publicreview - privatereview);
    printf("publicreview=%#p permissions=%#p diff=%tx\n", publicreview, permissions, permissions - publicreview);
    printf("realname=%#p permissions=%#p diff=%tx\n", realname, permissions, permissions - realname);
#else
    printf("\nusername=%#p realname=%#p diff=%tx\n", username, realname, realname - username);
    printf("realname=%#p privatereview=%#p diff=%tx\n", realname, privatereview, privatereview - realname);
    printf("privatereview=%#p publicreview=%#p diff=%tx\n", privatereview, publicreview, publicreview - privatereview);
    printf("publicreview=%#p permissions=%#p diff=%tx\n", publicreview, permissions, permissions - publicreview);
    printf("realname=%#p permissions=%#p diff=%tx\n", realname, permissions, permissions - realname);
#endif

    assert((size_t)(username + smallsz) <= (size_t)realname); 
    assert((size_t)(privatereview + biggersz) <= (size_t)publicreview);
    assert((size_t)(realname + smallsz) <= (size_t)permissions); 

// If we are in a CHERI environment, protect the review by setting
// the relevant struct members (in this case all of them)
// to read-only.  
bool b_improved = false;
  
#ifdef __CHERI_PURE_CAPABILITY__
    if(can_write(&review) == false) {
        printf("\nSetting the review to read-only.\n");
        struct review *ro_review = set_read_only(&review);
        assert((cheri_perms_get(ro_review) & CHERI_PERM_STORE) == 0);

        printf("\nThe struct is read-only so trying to change the review will make the program crash...\n");
        fflush(stdout);
        
        b_improved = try_improve_publicreview(ro_review, biggersz, false);
        
        // This line should be unreachable.
        print_details(ro_review);
    }
# else
    // Try editing the public review in a non-CHERI environment,
    // by simply checking the write permissions and refusing if none are found.
    b_improved = try_improve_publicreview(&review, biggersz, false);
#endif

    // This code will be unreachable in a CHERI environment,
    // but it would crash whether or not the struct had been set to read-only.
    if(b_improved == false) {
        printf("\nOverflowing reviewer realname by 1\n");
        memset(review.realname + smallsz+1, 'w', 1);
    
        // Now we can see the changes, if any. The permissions should have changed.
        print_details(&review);
    
        // Overflow the realname even more, which in a CHERI
        // environment should crash if it hasn't already.
        const size_t oversz = review.permissions - review.realname + 2 - smallsz;
        printf("\nOverflowing reviewer realname by %zx\n", oversz);
        memset(review.realname + smallsz+2, 'w', oversz);
    
        print_details(&review);
        
        b_improved = try_improve_publicreview(&review, biggersz, false);
        
        print_details(&review);
    }

    return 0;
}


