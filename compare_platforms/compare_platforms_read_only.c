/** 
 * This version can be run in both a CHERI and a non-CHERI environment to 
 * compare the two.
 * 
 * In a CHERI environment, it will:
 *     Step 1: Use can_read to check if the struct's "permissions" member includes write permissions.
 *     Step 2: Use capabilities to enforce read-only permissions
 *             (see https://github.com/capablevms/cheri-examples/blob/master/employee/include/employee.h).
 *     Step 3: Either:
 *             a) if command-line flag "-overwrite" was passed in, try to write to the read-only struct members, and SIGPROT; or
 *             b) try to exceed the bounds of an object and add write permissions (which will also SIGPROT).
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
    char *realname;
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
    printf("Reviewer's Real Name: %s \n", rv->realname);
    printf("Public Review: %s\n", rv->publicreview);
    printf("Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

struct review *set_read_only(struct review *rv)
{
    #ifdef __CHERI_PURE_CAPABILITY__
        rv->realname = (char *) cheri_perms_and(rv->realname, CHERI_PERM_LOAD);
        rv->publicreview = (char *) cheri_perms_and(rv->publicreview, CHERI_PERM_LOAD);
        rv->permissions = (char *) cheri_perms_and(rv->permissions, CHERI_PERM_LOAD);
        return (struct review *) cheri_perms_and(rv, CHERI_PERM_LOAD | CHERI_PERM_LOAD_CAP);
    #else
        assert(false);
    #endif
}

void overflow_reviewer_realname(size_t offset, size_t reps, struct review *rv) 
{
    printf("\nOverflowing reviewer realname by %zx\n", reps);
    memset(rv->realname + offset, 'w', reps);
    printf("Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

char* min(char* a, char* b)
{
    return(a > b ? b : a);
}

char* max(char* a, char* b)
{
    return(a > b ? a : b);
}

int main(int argc, char* argv[])
{
    struct review review;
    // In a non-CHERI environment, write protection is weakly enforced
    bool bWeak = true;
    
    // initialization values for struct member sizes
    const size_t smallsz = 0x20;
    const size_t biggersz = 0x1001;
       
    char *realname = malloc(smallsz);
    strcpy(realname, "Baba Yaga");
    review.realname = realname;
    
    char *publicreview = malloc(biggersz);
    strcpy(publicreview, "I am a little unclear as to the contribution. I think the authors could strengthen their case considerably if they conducted an RCT. Weak reject.");
    review.publicreview = publicreview;
    
    char *permissions = malloc(smallsz);
    strcpy(permissions, "r");
    review.permissions = permissions;
    
    print_details(&review);
    
#ifdef __CHERI_PURE_CAPABILITY__
    printf("smallsz=%zx, CRRL(smallsz)=%zx\n", smallsz,
        __builtin_cheri_round_representable_length(smallsz));
    printf("biggersz=%zx, CRRL(biggersz)=%zx\n", biggersz,
        __builtin_cheri_round_representable_length(biggersz));
    printf("publicreview=%#p realname=%#p diff=%tx\n", publicreview, realname, realname - publicreview);
    printf("realname=%#p permissions=%#p diff=%tx\n", realname, permissions, permissions - realname);
#else
    printf("publicreview=%p realname=%p diff=%tx\n", publicreview, realname, realname - publicreview);
    printf("realname=%p permissions=%p diff=%tx\n", realname, permissions, permissions - realname);
#endif
    
    bool b_improved = false;
    char *newpublicreview = malloc(biggersz);
    strcpy(newpublicreview, "5 ***** strong accept. This author deserves two Nobels and an ice cream.\n");

#ifdef __CHERI_PURE_CAPABILITY__    
    if(can_write(&review) == false) 
    {
        struct review *ro_review = set_read_only(&review);
        assert((cheri_perms_get(ro_review) & CHERI_PERM_STORE) == 0);

        if ((argc > 1) && (strcmp(argv[1], "-overwrite") == 0)) 
        {
            printf("\nThe struct is read-only so trying to change the review will make the program crash.\n");
            bWeak = false;
            b_improved = change_publicreview(ro_review, newpublicreview, bWeak);
            // This line should be unreachable.
            print_details(ro_review);
        }
        fflush(stdout);
        b_improved = false;
    }
#else
    b_improved = change_publicreview(&review, newpublicreview, true);
#endif 

    // Contrast the behaviour of this code in a CHERI vs. non-CHERI environment.
    if(b_improved == false) 
    {
        overflow_reviewer_realname(smallsz+1, 1, &review);
    
        const size_t oversz = review.permissions - review.realname + 2 - smallsz;
        overflow_reviewer_realname(smallsz+2, oversz, &review);
        
        // If we reached this line, we should have acquired write privileges on the review.
        b_improved = change_publicreview(&review, newpublicreview, bWeak);
        print_details(&review);
    }
    
    free(realname);
    free(publicreview);
    free(permissions);

    return 0;
}
