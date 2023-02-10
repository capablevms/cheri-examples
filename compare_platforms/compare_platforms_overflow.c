/*
 * This example demonstrates a simple overflow of a pointer that has had
 * 0x1001 bytes allocated using malloc(). It shows the expected behaviours
 * in three different CHERI environments, displaying a message to the user
 * immediately before the program is expected to crash in each respective
 * environment:
 *
 * - riscv64-purecap
 * - morello-purecap
 * - morello-hybrid
 *
 * The program can also be compiled and run in a non-CHERI environment,
 * but the behaviour is undefined, so the point at which the program crashes
 * (or whether it crashes) is unpredictable.
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
    char *publicreview;
    char *permissions;
};

void print_details(struct review *rv)
{
    printf("Public Review: %s\n", rv->publicreview);
    printf("Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

void overflow_publicreview(size_t offset, size_t reps, size_t overflow_amt, struct review *rv)
{
    printf("\nOverflowing publicreview by %zx\n", overflow_amt);
    memset(rv->publicreview + offset, 'w', reps);
    printf("Permissions: %s\n", rv->permissions);
    fflush(stdout);
}

char *min(char *a, char *b)
{
    return (a > b ? b : a);
}

char *max(char *a, char *b)
{
    return (a > b ? a : b);
}

void assert_positions(char *publicreview, char *permissions, size_t smsz, size_t bigsz)
{
    if (min(publicreview, permissions) == publicreview)
    {
        assert((publicreview + bigsz) <= permissions);
    }
    else
    {
        assert((permissions + smsz) <= publicreview);
    }
}

void free_pointers(struct review *rv)
{
    free(rv->publicreview);
    free(rv->permissions);
}

int main(int argc, char *argv[])
{
    // initialization values for struct member sizes
    const size_t smallsz = 0x20;
    const size_t biggersz = 0x1001;

    struct review review;

    char *publicreview = malloc(biggersz);
    review.publicreview = strcpy(
        publicreview, "I am a little unclear as to the contribution. I think the authors could "
                      "strengthen their case considerably if they conducted an RCT. Weak reject.");

    char *permissions = malloc(smallsz);
    review.permissions = strcpy(permissions, "r");

    print_details(&review);

    // Just to prove that the allocated spaces of the pointers are not
    // overlapping before we attempt to overflow them:
    assert_positions(publicreview, permissions, smallsz, biggersz);

#if (!defined(__CHERI_CAPABILITY_WIDTH__) && !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nIn a non-CHERI environment, you should reach this line, after which "
           "one of the attempts to overflow may or may not cause a crash.\n"
           "This behaviour is undefined, so the timing of the crash is unpredictable, "
           "if it happens at all.\n");
#endif

#if ((defined(__CHERI_PURE_CAPABILITY__)) && (defined(__aarch64__)))
    printf("\nOn morello-purecap, you should reach this line, then the attempt to overflow by 1 "
           "should "
           "cause a SIGPROT.\n");
    printf("This is because the representable length of 0x%zx bytes is 0x%zx.\n", biggersz,
           __builtin_cheri_round_representable_length(biggersz));
#endif

    size_t offset = biggersz + 1;
    overflow_publicreview(offset, 1, 1, &review);

    offset++;
    const size_t oversz = review.permissions - review.publicreview + 2 - biggersz;

#if ((defined(__CHERI_PURE_CAPABILITY__)) && (defined(__riscv)))
    size_t riscv64_padding = (__builtin_cheri_round_representable_length(biggersz) - offset) + 1;
    size_t overflow_amt = ((offset - biggersz) + riscv64_padding);
    printf("\nOn riscv64-purecap, you should reach this line, then the attempt to overflow by %zx "
           "should "
           "cause a SIGPROT.\n",
           overflow_amt);
    printf("You got this far because the representable length of 0x%zx bytes is 0x%zx.\n", biggersz,
           __builtin_cheri_round_representable_length(biggersz));
    overflow_publicreview(offset, riscv64_padding, overflow_amt, &review);
#endif

    overflow_publicreview(offset, oversz, oversz, &review);

    // If we reached this line, we should have acquired write privileges on the review.
    print_details(&review);

    free_pointers(&review);

#if (defined(__aarch64__) && defined(__CHERI_CAPABILITY_WIDTH__) &&                                \
     !defined(__CHERI_PURE_CAPABILITY__))
    printf("\nOn morello-hybrid, you should make it all the way to the end without crashing.\n");
#endif

    return 0;
}
