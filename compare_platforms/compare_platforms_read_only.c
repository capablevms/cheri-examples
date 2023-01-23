/** 
 * This version can be run in both a CHERI and a non-CHERI environment to 
 * compare the two.
 * 
 * In a CHERI environment, it will:
 *     Step 1: Use can_read to check if the struct's "permissions" member includes write permissions.
 *     Step 2: Use capabilities to enforce read-only permissions
 *             (see https://github.com/capablevms/cheri-examples/blob/master/employee/include/employee.h).
 *     Step 3: Offer the user a choice to:
 *             a) try to write to the read-only struct members, and SIGPROT; or
 *             b) try to exceed the bounds of an object and add write permissions (which will crash).
 *
 * In a non-CHERI environment, it will:
 *     Step 1: Use can_read to check if the struct's "permissions" member includes write permissions.
 *     Step 2: Try to exceed the bounds of an object and add write permissions
 *             (see https://ctsrd-cheri.github.io/cheri-exercises/exercises/buffer-overflow-heap/index.html).
 *     Step 3: Rewrite the review.
 */

#ifdef __CHERI_PURE_CAPABILITY__
#include "../include/common.h"
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct review
{
    char *username;
    char *realname;
    char *privatereview;
    char *publicreview;
    char *permissions;
};

bool can_write(struct review *rv) {
    return (strchr(rv->permissions, 'w') != NULL);
}

bool change_publicreview(struct review *rv, char* newpublicreview, bool bWeak)
{
    if(can_write(rv) || (bWeak == false)) {
        printf("\nAttempting to change the public review.\n");
        rv->publicreview = newpublicreview;
        return true;
    }
    else {
        printf("\nYou don't have permission to change the public review.\n");
        return false;
    }
}

void print_details(struct review *rv)
{
    printf("\nReviewer Username: %s \n", rv->username);
    printf("Reviewer's Real Name: %s \n", rv->realname);
    printf("Private Review: %s\n", rv->privatereview);
    printf("Public Review: %s\n", rv->publicreview);
    printf("Review's Read-Write Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

struct review *set_read_only(struct review *rv)
{
    #ifdef __CHERI_PURE_CAPABILITY__
        rv->username = (char *) cheri_perms_and(rv->username, CHERI_PERM_LOAD);
        rv->realname = (char *) cheri_perms_and(rv->realname, CHERI_PERM_LOAD);
        rv->privatereview = (char *) cheri_perms_and(rv->privatereview, CHERI_PERM_LOAD);
        rv->publicreview = (char *) cheri_perms_and(rv->publicreview, CHERI_PERM_LOAD);
        rv->permissions = (char *) cheri_perms_and(rv->permissions, CHERI_PERM_LOAD);
        return (struct review *) cheri_perms_and(rv, CHERI_PERM_LOAD | CHERI_PERM_LOAD_CAP);
    #else
        assert(false);
    #endif
}


int main()
{
    struct review review;
    // In a non-CHERI environment, write protection is weakly enforced
    bool bWeak = true;
    
    size_t smallsz = 0x20;
    size_t biggersz = 0x1001;
    
    char *username = malloc(smallsz);
    strcpy(username, "Reviewer 2");    
    char *realname = malloc(smallsz);
    strcpy(realname, "Baba Yaga");
    char *privatereview = malloc(biggersz);
    strcpy(privatereview, "I cannot believe I read this appalling piece of dreck from start to finish. The authors should be ashamed, and I hope I get an opportunity to tell them so to their faces.");
    char *publicreview = malloc(biggersz);
    strcpy(publicreview, "I am a little unclear as to the contribution. I think the authors could strengthen their case considerably if they conducted an RCT. Weak reject.");
    char *permissions = malloc(smallsz);
    strcpy(permissions, "r");
    
    review.username = username;
    review.realname = realname;
    review.privatereview = privatereview;
    review.publicreview = publicreview;
    review.permissions = permissions;
    print_details(&review);
    
    assert((size_t)(username + smallsz) <= (size_t)realname); 
    assert((size_t)(privatereview + biggersz) <= (size_t)publicreview);
    assert((size_t)(realname + smallsz) <= (size_t)permissions);
    
    bool b_improved = false;
    char *newpublicreview = malloc(biggersz);
    strcpy(newpublicreview, "5 ***** strong accept. This author deserves two Nobels and an ice cream.\n");

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
    printf("\nusername=%p realname=%p diff=%tx\n", username, realname, realname - username);
    printf("realname=%p privatereview=%p diff=%tx\n", realname, privatereview, privatereview - realname);
    printf("privatereview=%p publicreview=%p diff=%tx\n", privatereview, publicreview, publicreview - privatereview);
    printf("publicreview=%p permissions=%p diff=%tx\n", publicreview, permissions, permissions - publicreview);
    printf("realname=%p permissions=%p diff=%tx\n", realname, permissions, permissions - realname);
#endif 
  
#ifdef __CHERI_PURE_CAPABILITY__
    if(can_write(&review) == false) {
        printf("\nSetting the review to read-only.\n");
        struct review *ro_review = set_read_only(&review);
        assert((cheri_perms_get(ro_review) & CHERI_PERM_STORE) == 0);

        printf("\nThe struct is read-only so trying to change the review will make the program crash.\n");
        printf("\nTry anyway? y/n: \n");
        char answer;
        scanf(" %c", &answer);
        if (answer == 'Y' || answer == 'y'){
            bWeak = false;
            b_improved = change_publicreview(ro_review, newpublicreview, bWeak);
            // This line should be unreachable.
            print_details(ro_review);
        }
        fflush(stdout);
        b_improved = false;
    }
# else
    b_improved = change_publicreview(&review, newpublicreview, true);
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
        
        b_improved = change_publicreview(&review, newpublicreview, bWeak);
        
        print_details(&review);
    }

    return 0;
}









