/**
 * Review struct and auxiliary functions to display review's details,
 * change the content, and operate on the struct's permissions.
 *
 * This is based on the employee example at
 * https://github.com/capablevms/cheri-examples/blob/master/employee/include/employee.h
**/

#ifdef __CHERI_PURE_CAPABILITY__
#include "../../include/common.h"
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Reviewer's username, real name, private comments, public review, and permissions. 
**/
struct review
{
    char *username;
    char *realname;
    char *privatereview;
    char *publicreview;
    char *permissions;
};

/**
 * Check this review's permissions.
 * param: review, the struct with the details of the review whose permissions we are checking
 * return: true if the review has write permissions, false otherwise.
**/
bool can_write(struct review *rv) {
    bool canwrite = true;
    
    // a rigorous protocol of looking for the letter w in the permissions
    char *pt = strchr(rv->permissions, 'w');
    if(pt == NULL)
        return false;

    return canwrite;
}

/**
 * A method for editing the member of the review structure that holds a public review.
 * param: rv, the struct representing the review in question
 * param: publicreview, a string with the public review intended to replace the existing one
 * return: false if review is read-only, true if review is writable.
**/
bool change_publicreview(struct review *rv, char* publicreview)
{
    #ifdef __CHERI_PURE_CAPABILITY__
        // If this is read only, it will crash.
        // To test this in a CHERI environment,
        // we won't do a weak check of read-only permissions here.
        rv->publicreview = publicreview;
        // If we got this far, it was writable.
        return true;
    #else
        if(can_write(rv)) {
            rv->publicreview = publicreview;
            return true;
        }
        else {
            printf("You don't have permission to change this public review.\n");
            return false;
        }
    #endif
}

/**
 * A method for attempting to change the public review to something more flattering.
 * param: rv, the struct representing the public review in question.
 * param: reqsz, a size_t representing the amount of memory to allocate for the new review.
 * param: b_weak, a boolean that is true if we are weakly enforcing (i.e. using can_write) the 
 * read-only permissions, false otherwise.
 * return: false if review is read-only, true if review is writable.
**/
bool try_improve_publicreview(struct review *rv, size_t reqsz, bool b_weak) {
    bool b_writable = false;
    
    char *newpublicreview = malloc(reqsz);
    strcpy(newpublicreview, "5 ***** strong accept. This author deserves two Nobels and an ice cream.\n");

    printf("\nAttempting to change the public review.\n");
    #ifdef __CHERI_PURE_CAPABILITY__
        if((can_write(rv) == false) && (b_weak == true)) {
            printf("You do not have permission to change the public review.\n");
            return false;
        } else {
            // if the review is set to read-only, this will crash.
            b_writable = change_publicreview(rv, newpublicreview);
            return b_writable;
        }
    #else
        b_writable = change_publicreview(rv, newpublicreview);
        return b_writable;
    #endif
}

/**
 * Print all the member values for a given review.
 * param: rv, the struct representing the review in question
**/
void print_details(struct review *rv)
{
    printf("\nReviewer Username: %s \n", rv->username);
    printf("Reviewer's Real Name: %s \n", rv->realname);
    printf("Private Review: %s\n", rv->privatereview);
    printf("Public Review: %s\n", rv->publicreview);
    printf("Review's Read-Write Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

/**
 * Get the actual length of a string being assigned to a member.
 * param: userdata, a char * to be assigned to a member of the review struct
 * param: explen, an int representing the allocated length
 * return: actsize, a size_t representing the actual size of the string
 **/
size_t actual_input_length(char *userdata, int explen) 
{
    size_t actsz = (size_t)strlen(userdata);
    assert(actsz > 0);
    assert(actsz <= explen);
    
    return actsz;
}



#ifdef __CHERI_PURE_CAPABILITY__
/**
 * Set all the struct members to read-only.
 * param: rv, the struct representing the review to display
 * return: a review with permissions restricted to read-only
 **/
struct review *set_read_only(struct review *rv)
{
    rv->username = (char *) cheri_perms_and(rv->username, CHERI_PERM_LOAD);
    rv->realname = (char *) cheri_perms_and(rv->realname, CHERI_PERM_LOAD);
    rv->privatereview = (char *) cheri_perms_and(rv->privatereview, CHERI_PERM_LOAD);
    rv->publicreview = (char *) cheri_perms_and(rv->publicreview, CHERI_PERM_LOAD);
    rv->permissions = (char *) cheri_perms_and(rv->permissions, CHERI_PERM_LOAD);
    return (struct review *) cheri_perms_and(rv, CHERI_PERM_LOAD | CHERI_PERM_LOAD_CAP);
}
#endif