/*
 * The purpose of this example is to compare how a stack-smashing attempt is handled
 * in a CHERI purecap vs hybrid environment, vs a non-CHERI environment.
 *
 * The example first displays a substring of the Latin alphabet starting from a random index,
 * then tries to smash the stack using an index that is known to be out-of-bounds,
 * and display the contents of the out-of-bounds address to the user.
 *
 * In a CHERI purecap environment:
 * - The program should crash with a SIGPROT caused by a capability bounds fault;
 * - The user should not see the contents of the out-of-bounds address;
 * - The address of the pointer should change when its value changes.
 *
 * In a morello-hybrid environment:
 * - The program should run without crashing;
 * - The user should see the contents of the out-of-bounds address;
 * - The address of the pointer should not change when its value changes.
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

void display_process_info(char *str)
{
    printf("\nPID = %d\n", getpid());

#ifdef __CHERI_PURE_CAPABILITY__    
    pp_cap(str);
#else
    printf("Address of str = %p\n", &str);
#endif
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
    
    display_process_info(str);

    // Show the audience what they should be seeing
    for (int i = 0; i < 10; i++)
    {
        sleep(1);
        printf("%s\n", str);
        fflush(stdout);
    }

    // get an index we know is out-of-bounds of alphabet
    const size_t twice_alphabet_size = 52;
    size_t evilindex = twice_alphabet_size - goodindex;
    // change the message to something the user shouldn't see
    str = &alphabet[evilindex];

    display_process_info(str);

    // now show the ill-gotten string
    for (int i = 0; i < 10; i++)
    {
        sleep(1);
        // In a purecap environment, this is what causes a SIGPROT.
        // We know the address of str is off the end of alphabet because we put it there.
        // Specifically, a call to strlen() is on the stack at the point of the SIGPROT,
        // and it results in a capability bounds fault.
        printf("%s\n", str);
        fflush(stdout);
    }

    return 0;
}
