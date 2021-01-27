/* xor_lists.c
 * Jeremy Singer
 * Example of doubly-linked list data structure
 * featuring xor'd pointers for shared next/prev field.
 *
 * This is an example of pointer fiddling code that
 * currently _won't_ work on CHERI platforms, since
 * the XOR operation on a capability probably puts
 * the pointer out of bounds (invalidating the
 * capability so it cannot be dereferenced).
 *
 * Related Reading Material:
 * 1) Wikipedia article on XOR linked list
 *     https://en.wikipedia.org/wiki/XOR_linked_list
 * 2) OOPSLA 2020 paper on
 *     "Sound Garbage Collection for C using Pointer Provenance"
 *     https://dl.acm.org/doi/10.1145/3428244
 * 3) Complete spatial safety for C and C++ using
 *    CHERI capabilities.
 *     https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-949.pdf
	   See Section 3.9.1 XOR-linked lists (p56)
 */

#include <stdio.h>
#include <stdlib.h>

#define NUM_CELLS 100
#define WORD long int

/* linked list cell has two fields
 *   data is cell payload
 *   ptr is superposition of next/previous pointers,
 *    which are XOR'd together
 */
typedef struct cell
{
	WORD data;
	WORD ptr;
} cell_t;

cell_t *alloc_cell(WORD payload, cell_t *tail)
{
	cell_t *cell = (cell_t *)malloc(sizeof(cell_t));
	cell->data = payload;
	/* xor this cell's address into the tail's ptr field */
	if (tail != NULL)
		tail->ptr ^= (WORD)cell;
	/* store tail address in this cell's ptr field
	 * NOTE most recently allocated cell (i.e. head)
	 * only stores a single ptr, not an XOR'd pair
	 */
	cell->ptr = (WORD)tail;
	return cell;
}

int main()
{
	int i = 0;
	cell_t *head = NULL;
	cell_t *prev;
	cell_t *curr;
	cell_t *next;
	for (i = 0; i < NUM_CELLS; i++)
	{
		/* allocate a cell */
		head = alloc_cell(i, head);
	}
	/* now we have NUM_CELLS cells in a doubly-linked list */
	/* traverse list from head to tail */
	printf("%ld\n", head->data);
	prev = head;
	curr = (cell_t *)prev->ptr;
	while (curr != NULL)
	{
		printf("%ld\n", curr->data);
		// chase tail
		next = (cell_t *)((curr->ptr) ^ (WORD)prev);
		// move along one
		prev = curr;
		curr = next;
	}
	/* now we are at the NULL ptr at the end of the list */
	/* let's traverse back to the start of the list */
	next = curr;
	curr = prev;
	prev = (cell_t *)((curr->ptr) ^ (WORD)next);
	while (curr != head)
	{
		printf("%ld\n", curr->data);
		// chase reverse tail
		next = curr;
		curr = prev;
		prev = (cell_t *)((curr->ptr) ^ (WORD)next);
	}
	printf("%ld\n", head->data);

	return 0;
}
