/**********************************
 * bump_alloc.h
 * Jeremy.Singer@glasgow.ac.uk
 *
 * This is a simple bump-pointer allocator.
 * It mmaps a large buffer of SIZE bytes,
 * then allocates this space in word-sized
 * chunks to client code (in main fn).
 *
 * Initial simple memory allocator test.
 * Explore capability narrowing operations
 * and intrinsics for bound reporting.
 */

void init_alloc(int size_in_bytes);

char *bump_alloc(int bytes); /* the simplest malloc */
                             /* oh, and there's no free() ! */
