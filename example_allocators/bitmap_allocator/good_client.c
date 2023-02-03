/*
 * good_client.c
 * Jeremy Singer
 *
 * Example use of bitmap allocator, showing how
 * 'good' client code should interact with the
 * allocator.
 */

#include <cheri/cheric.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap_alloc.h"

/* should program generate debugging output? 1 for Yes, 0 for No */
#define DEBUG_PRINTF 1

/* size of buffer to allocate (in list cells) */
#define NUM_CELLS 64

/* trivial linked list data structure */
typedef struct _list
{
    int payload;
    struct _list *next;
    ;
} list;

int main()
{
    unsigned int i = 0;
    unsigned int n = 1;
    list *l = NULL;

    init_alloc(NUM_CELLS, sizeof(list));
    while (n <= NUM_CELLS)
    {
        /* now try to do some allocations */
        for (i = 0; i < n; i++)
        {
            list *tmp = (list *) alloc_chunk();
            if (tmp)
            {
                tmp->payload = i;
                tmp->next = l;
                printf("alloc cell %d\n", i);
                l = tmp;
            }
            else
            {
                fprintf(stderr, "chunk allocation failed but there should be enough memory!");
                exit(-1);
            }
        }

        // now free this list
        while (l != NULL)
        {
            list *tmp = l->next;
            printf("free cell %d\n", l->payload);
            free_chunk(l);
            l = tmp;
        }

        /* double the limit, n */
        n <<= 1;
    }

    return 0;
}
