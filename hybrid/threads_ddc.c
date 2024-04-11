/***
 * This example explores setting different DDCs across threads, ensuring each
 * thread has its own DDC which does not interfere with other threads.
 *
 * The number of threads to execute can be changed by modifying `t_count`.
 ***/

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cheriintrin.h"

#include "../include/common.h"
#include "./include/utils.h"

struct t_info
{
    unsigned short count;
    pthread_t tid;
    void *addr_in;
    void *__capability ddc_out;
};

void *run_thread(void *curr_t_info_ptr)
{
    struct t_info *curr_t_info = (struct t_info *) curr_t_info_ptr;
    void *__capability old_ddc = read_ddc();
    void *__capability new_ddc =
        cheri_address_set(cheri_ddc_get(), (intptr_t) curr_t_info->addr_in);
    write_ddc(new_ddc);
    // Force wait to ensure threads don't die too fast
    for (size_t i = 0; i < pow(10, 6); ++i)
    {
    };
    curr_t_info->ddc_out = read_ddc();
    assert(cheri_address_get(curr_t_info->ddc_out) == (unsigned long) curr_t_info->addr_in);
    assert(curr_t_info->ddc_out != old_ddc);
    return NULL;
}

int main()
{
    unsigned short t_count = 3;
    const unsigned int to_alloc = 256;
    struct t_info *t_infos = malloc(t_count * sizeof(struct t_info));
    pthread_t *tids = malloc(t_count * sizeof(pthread_t));

    for (size_t i = 0; i < t_count; ++i)
    {
        t_infos[i].count = i;
        t_infos[i].addr_in = malloc(to_alloc);
        pthread_create(&t_infos[i].tid, NULL, &run_thread, &t_infos[i]);
    }

    printf("== Main thread DDC:\n");
    pp_cap(cheri_ddc_get());
    for (size_t i = 0; i < t_count; ++i)
    {
        pthread_join(t_infos[i].tid, NULL);
        printf("== From thread %hu:\n", t_infos[i].count);
        pp_cap(t_infos[i].ddc_out);
        free(t_infos[i].addr_in);
    }

    free(tids);
    free(t_infos);
    return 0;
}
