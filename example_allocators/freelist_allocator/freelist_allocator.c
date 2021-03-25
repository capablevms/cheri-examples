#include "freelist_allocator.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

char *small_freelist = NULL;

char *medium_freelist = NULL;

char *large_freelist = NULL;

void initialize(unsigned int size_in_pages)
{
	/* request memory for our allocation buffer
	 * NB mmap min bounds for capability is 1 page (4K)
	 */
	size_t bytes_to_allocate = size_in_pages * BYTES_IN_PAGE;
	char *res =
		mmap(NULL, bytes_to_allocate, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (res == MAP_FAILED)
	{
		perror("error in initial mem allocation");
		exit(-1);
	}

	// put in linked list pointers and
	// stick into the large freelist
	// give this space to the large freelist ...
	large_freelist = insert_linked_list_pointers(LARGE, bytes_to_allocate, res, large_freelist);
	return;
}

char *insert_linked_list_pointers(size_t cell_size, size_t limit, char *start, char *freelist)
{
	char *curr = start;
	char *next = curr + cell_size;
	char *max = start + limit;
	// ensure next ptr will fit into cell
	assert(sizeof(void *) <= cell_size);

	while (next < max)
	{
		((char **)curr)[0] = next;
		curr = next;
		next = curr + cell_size;
	}
	// at the end, concatenate this newly formed
	// list with existing freelist
	((char **)curr)[0] = freelist;

	return start;
}

char *alloc(size_t bytes)
{

	size_t size;
	char *freelist_to_use = NULL;
	char *ret = NULL; // ptr to return

	// work out which freelist to use
	if (bytes <= SMALL)
	{
		size = SMALL;
		freelist_to_use = small_freelist;
	}
	else if (bytes <= MEDIUM)
	{
		size = MEDIUM;
		freelist_to_use = medium_freelist;
	}
	else
	{
		size = LARGE;
		freelist_to_use = large_freelist;
	}

	if (freelist_to_use == NULL)
	{
		// fixup freelist (if no available mem there)
		char *new_space = NULL;
		switch (size)
		{
		case SMALL:
			new_space = alloc(MEDIUM);
			if (new_space != NULL)
			{
				small_freelist =
					insert_linked_list_pointers(SMALL, MEDIUM, new_space, small_freelist);
				freelist_to_use = small_freelist;
				// now we have replenished space...
			}
			break;
		case MEDIUM:
			new_space = alloc(LARGE);
			if (new_space != NULL)
			{
				medium_freelist =
					insert_linked_list_pointers(MEDIUM, LARGE, new_space, medium_freelist);
				freelist_to_use = medium_freelist;
			}
			break;
		default:
			// stuck! no more mem!
			// we will return NULL
			break;
		}
	}

	// pop from head of freelist (if there's anything there)
	if (freelist_to_use != NULL)
	{
		char *head = freelist_to_use;
		char *tail = ((char **)head)[0];
		switch (size)
		{
		case SMALL:
			small_freelist = tail;
			break;
		case MEDIUM:
			medium_freelist = tail;
			break;
		default:
			large_freelist = tail;
			break;
		}
		ret = head;
		SET_SIZE(ret, size);
	}
	return ret;
}

void dealloc(void *buffer)
{

	// work out the size of the buffer
	size_t size;
	char *freelist;

	size = GET_SIZE(buffer);

	// then prepend it to the appropriate freelist
	switch (size)
	{
	case SMALL:
		small_freelist = cons_onto_freelist(buffer, small_freelist);
		break;
	case MEDIUM:
		medium_freelist = cons_onto_freelist(buffer, medium_freelist);
		break;
	default:
		large_freelist = cons_onto_freelist(buffer, large_freelist);
		break;
	}

	return;
}

char *cons_onto_freelist(char *cell, char *freelist)
{
	((char **)cell)[0] = freelist;
	return cell;
}
