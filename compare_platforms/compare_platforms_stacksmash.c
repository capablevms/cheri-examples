/*
 * Usage:
 * prompt> ./cpu_random & ./cpu_random & ./cpu_random & ./cpu_random &
 * 
 * This program declares two character pointers of equal length: 
 * first the Latin alphabet,
 * then an evil message that the user should not see.
 * 
 * The program then selects a substring from the alphabet, starting at a random index,
 * and displays it to the user 10 times.
 *
 * Next, the program attempts to smash the stack:
 * It selects another random index that is certain to be out of bounds of the alphabet
 * pointer, tries to get a substring from this out-of-bounds index, and tries to display
 * this ill-gotten substring to the user.
 * 
 * The program is intended to be compiled and run in a CHERI or non-CHERI environment,
 * and the outputs compared. The points of comparison are:
 * - Does the program crash on the attempt to smash the stack?
 * - Does the user see the evil message that they should not see?
 * - When does the address of str change and when does it stay the same?
 * 
 * In a purecap environment, you should see the following:
 
 * - A single instance of the program should crash; multiple instances should hang.
 * -- After the user manually kills one of the instances, the rest should follow without
 * user intervention.
 * - The user should not see the evil message.
 * - The capability should stay the same throughout, but the address of str should be
 * the same for all instances having the same value of str, and change when str's value
 * changes to the evil message. 
 * -- The value of str will usually, but not always, be the same for all instances.
 * -- The capability should stay the same throughout.
 * -- The address of the evil message will be outside the capability's bounds.
 *
 * In morello-hybrid, you should see the following:
 * - The program should not crash or hang.
 * - The user should see the evil message.
 * - The address of str should be different for each respective instance, and should not
 * change when str's value changes to the evil message.
 *
*/

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include "../include/common.h"
#endif

intmax_t get_time() 
{
    struct timeval t;
    intptr_t rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (intmax_t) t.tv_sec + (intmax_t) t.tv_usec/1e6;
}

void spin(intptr_t howlong) 
{
    intmax_t t = get_time();
    while ((get_time() - t) < (intmax_t) howlong)
        ; // do nothing in loop
}

int main(int argc, char *argv[])
{    
    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *evilmessage = "mine is an evil laugh muaha";
    srand((unsigned)time(NULL));
    
    const size_t alphabet_size = 26;
    size_t goodindex = rand() % alphabet_size;
    char *str = &alphabet[goodindex];
    
#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nPID = %d\n", getpid());
    pp_cap(str);
#else
    printf("\nPID = %d\n", getpid());
    printf("Address of str = %p\n", &str);
#endif
    
    for(int i=0; i<10; i++) {
        printf("%s\n", str);
        spin(1);
    }

    const size_t twice_alphabet_size = 52;
    size_t evilindex = twice_alphabet_size - goodindex;
    //char *evilstr = &alphabet[evilindex];
    str = &alphabet[evilindex];
    
#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nPID = %d\n", getpid());
    pp_cap(str);
#else
    printf("\nPID = %d\n", getpid());
    printf("Address of str = %p\n", &str);
#endif

    while (1) {
        printf("%s\n", str);
        spin(1);
    }
    
    return 0;
}

