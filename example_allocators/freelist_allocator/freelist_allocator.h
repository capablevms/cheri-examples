/**********************************
 * freelist_allocator.h
 * Jeremy.Singer@glasgow.ac.uk
 *
 * This is a simple segregated freelist allocator.
 * It mmaps a large buffer of num_pages pages,
 * then constructs a linked list of LARGE-sized cells.
 *
 * When an alloc request occurs, we have three size
 * classes we can use - SMALL, MEDIUM, and LARGE.
 * If there is a empty cell available in the appropriate
 * freelist, we return this cell.
 * If there are no cells available, we try to
 * grab a cell from a larger freelist to replenish
 * our freelist, and return one of these cells.
 * If there is no memory available, alloc returns NULL.
 *
 * When a dealloc request occurs, we know the size
 * of the cell so we can prepend the cell onto the
 * appropriate freelist.
 *
 * NB Allocated cells have their sizes encoded in the
 * corresponding cell capability - this means we
 * naively assume that  allocator client code
 * does _not_ interfere with the capability
 * metadata.
 */

#include <cheriintrin.h>
#include <stddef.h>

/* possible sizes for cells */
#define SMALL 16
#define MEDIUM 256
#define LARGE 4096

/* we assume 4K pages */
#define BYTES_IN_PAGE LARGE

/* cell sizes encoded in CHERI bounds metadata */
#define SET_SIZE(cell, size) cell = cheri_bounds_set_exact(cell, size)
#define GET_SIZE(cell) cheri_length_get(cell)

/* allocator init routine */
void initialize(unsigned int num_pages);

/* malloc and free */
char *alloc(size_t bytes);
void dealloc(void *buffer);

/* freelist management routines */
char *insert_linked_list_pointers(size_t cell_size, size_t limit, char *start, char *freelist);
char *cons_onto_freelist(char *cell, char *freelist);
