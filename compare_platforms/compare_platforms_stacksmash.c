/*
 * This example is meant to be run in multiple instances launched simultaneously.
 * It can also be run as a single instance.
 *
 * Usage:
 * prompt> ./compare_platforms_stacksmash & ./compare_platforms_stacksmash &
 * or:
 * prompt> ./compare_platforms_stacksmash
 *
 * The purpose of the example is to compare how a stack-smashing attempt is handled
 * in a CHERI purecap vs hybrid environment, vs a non-CHERI environment.
 *
 * The example first displays a substring of the Latin alphabet starting from a random index,
 * then tries to smash the stack using an index that is known to be out-of-bounds,
 * and display the out-of-bounds substring to the user.
 *
 * In a CHERI purecap environment:
 * - A single instance should crash, and multiple instances should hang;
 * - The user should not see the out-of-bounds substring;
 * - The address of the pointer should be the same for all instances but change when
 * the substring's value changes.
 *
 * In a morello-hybrid environment:
 * - The program should run without crashing;
 * - The user should see the out-of-bounds substring;
 * - The address of the pointer should be different for all instances and should not
 * change when the substring's value changes.
 *
 * In a non-CHERI environment, the program's behaviour is implementation dependent.
 */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include "../include/common.h"
#endif

intmax_t get_time()
{
    struct timeval t;
    intptr_t rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (intmax_t) t.tv_sec + (intmax_t) t.tv_usec / 1e6;
}

void spin(intptr_t howlong)
{
    intmax_t t = get_time();
    while ((get_time() - t) < (intmax_t) howlong)
        ;
}

int main(int argc, char *argv[])
{
    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *evilmessage = "mine is an evil laugh muaha";
    srand((unsigned) time(NULL));

    // get an index we know is within bounds of alphabet
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

    // Show the audience what they should be seeing
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", str);
        spin(1);
    }

    // get an index we know is out-of-bounds of alphabet
    const size_t twice_alphabet_size = 52;
    size_t evilindex = twice_alphabet_size - goodindex;
    // change the message to something the user shouldn't see
    str = &alphabet[evilindex];

#ifdef __CHERI_PURE_CAPABILITY__
    printf("\nPID = %d\n", getpid());
    pp_cap(str);
#else
    printf("\nPID = %d\n", getpid());
    printf("Address of str = %p\n", &str);
#endif

    // keep showing the ill-gotten string until the user kills the process
    for (int i = 0; i < 10; i++)
    {
        printf("%s\n", str);
        spin(1);
    }

    return 0;
}
