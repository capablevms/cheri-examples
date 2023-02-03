#ifndef __MMAP_SHARED_VS_PRIVATE_UTIL_H__
#define __MMAP_SHARED_VS_PRIVATE_UTIL_H__

#include <stddef.h>
#include <stdint.h>

#define MMAP_SIZE 4096
#define INIT_TEST_CASE(prot_in, flags_in, fd_in)                                                   \
    {                                                                                              \
        .prot = prot_in, .flags = flags_in, .fd = fd_in,                                           \
        .details = "flags = " #flags_in ", prot = " #prot_in                                       \
    }

typedef struct mmap_test_case
{
    /* The memory protection flags */
    int prot;
    /* The visibility flags */
    int flags;
    /* The file descriptor, if a file mapping was requested. */
    int fd;
    /* A string describing the mapping (used for debugging). */
    char *details;
} mmap_test_case_t;

int run_mmap_test(mmap_test_case_t);

#endif // __MMAP_SHARED_VS_PRIVATE_UTIL_H__
