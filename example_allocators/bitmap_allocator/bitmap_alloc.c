/**********************************
 * bitmap_alloc.c
 * Jeremy.Singer@glasgow.ac.uk
 *
 * This is a simple fixed-size bitmap allocator.
 * It mmaps a large buffer of
 * NUM_CHUNKS * CHUNK_SIZE bytes
 * then allocates this space in equally-sized
 * chunks to client code.
 * A side bitmap is required to keep track of which
 * chunks are in use (corresponding bit set to 1)
 * and which chunks are free (corresponding bit
 * set to 0). There is one bit per allocatable chunk.
 *
 * This is _not_ a clever allocator, since it
 * does a linear scan of the bitmap to find the
 * first free chunk, which is expensive!
 * More efficient scans could be easily incorporated.
 *
 * This is _not_ a general-purpose allocator, since
 * it only allocates chunks of a fixed size. Further,
 * this size is constrained to be small enough to allow
 * exact bounds representation in CHERI capabilities.
 *
 * This is an initial simple memory allocator test
 * for CHERI / Capable VMs.
 * We explore capability alignment,
 * representable bounds, narrowing operations
 * and compiler intrinsic support.
 */

#include <assert.h>
#include <cheriintrin.h>

#include <cheri/cheric.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "bitmap_alloc.h"

#define BITS_PER_BYTE 8

char *buffer = NULL;          /* allocation buffer */
unsigned char *bitmap = NULL; /* bitmap for the buffer */

int buffer_size = 0;     /* size of buffer (in bytes) */
int bitmap_size = 0;     /* size of bitmap (in bytes) */
int bytes_per_chunk = 0; /* size of single chunk (in bytes) */

void init_alloc(int num_chunks, int chunk_size)
{
    int i = 0;

    /* we need to increase the num_chunks
     * so every bit in bitmap will be used
     */
    int adjusted_num_chunks = (num_chunks % BITS_PER_BYTE == 0)
                                  ? num_chunks
                                  : (num_chunks + (BITS_PER_BYTE - (num_chunks % BITS_PER_BYTE)));

    /* we need to increase the chunk_size
     * so chunks will be CHERI aligned
     * (i.e. 16 bytes for RISC-V 64-bit arch)
     */
    int adjusted_chunk_size =
        (chunk_size % (sizeof(void *)) == 0)
            ? chunk_size
            : (chunk_size + (sizeof(void *)) - (chunk_size % (sizeof(void *))));

    /* check this chunk size is small enough so we can represent
     * bounds precisely with CHERI compressed representation
     */
    adjusted_chunk_size = cheri_representable_length(adjusted_chunk_size);

    /* request memory for our allocation buffer */
    char *res = mmap(NULL, adjusted_num_chunks * adjusted_chunk_size, PROT_READ | PROT_WRITE,
                     MAP_ANON | MAP_PRIVATE, -1, 0);
    /* request memory for our bitmap */
    bitmap = (unsigned char *) mmap(NULL, adjusted_num_chunks / BITS_PER_BYTE,
                                    PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (res == MAP_FAILED || bitmap == MAP_FAILED)
    {
        perror("error in initial mem allocation");
        exit(-1);
    }

    /* NB mmap min bounds for capability is 1 page (4K) */
    buffer = res;
    /* check buffer is aligned */
    assert((uintptr_t) buffer % sizeof(void *) == 0);
    /* check bitmap is aligned */
    assert((uintptr_t) bitmap % sizeof(void *) == 0);

    bytes_per_chunk = adjusted_chunk_size;
    buffer_size = adjusted_num_chunks * adjusted_chunk_size;
    bitmap_size = adjusted_num_chunks / BITS_PER_BYTE;

    /* zero bitmap, since all chunks are free initially */
    for (i = 0; i < bitmap_size; i++)
    {
        bitmap[i] = 0;
    }

    // set exact bounds for buffer and bitmap?
    buffer = cheri_setbounds(buffer, buffer_size);
    bitmap = cheri_setbounds(bitmap, bitmap_size);
    return;
}

/*
 * allocate fixed size chunk with bitmap allocator
 * this is our simplistic `malloc` function
 */
char *alloc_chunk()
{
    unsigned char updated_byte = 0;
    int chunk_index = 0;
    char *chunk = NULL;
    // iterate over all bits in bitmap, looking for a 0
    // when we find a 0, set it to 1 and
    // return the corresponding chunk
    // (setting its capability bounds)
    int i = 0;
    while (bitmap[i] == (unsigned char) 0xff)
    {
        i++;
        if (i >= bitmap_size)
            break;
    }
    // do we have a 0?
    if (i < bitmap_size && bitmap[i] != (unsigned char) 0xff)
    {
        // find the lowest 0 ...
        int j = 0;
        // right shift until bottom bit is 0
        for (j = 0; j < BITS_PER_BYTE; j++)
        {
            int bit = (bitmap[i] >> j) & 1;
            if (bit == 0)
            {
                break;
            }
        }
        // now i is the word index, j is the bit index
        // set this bit to 1 ...
        // and work out the chunk to allocate
        updated_byte = bitmap[i] + (unsigned char) (1 << j);
        bitmap[i] = updated_byte;

        chunk_index = i * BITS_PER_BYTE + j;
        chunk = buffer + (chunk_index * bytes_per_chunk);

        /* restrict capability range before returning ptr */
        chunk = cheri_setbounds(chunk, bytes_per_chunk);
    }

    return chunk;
}

void free_chunk(void *chunk)
{
    vaddr_t base = cheri_getbase(chunk);
    vaddr_t buff_base = cheri_getbase(buffer);
    /* calculate chunk index in buffer */
    int chunk_index = (base - buff_base) / bytes_per_chunk;
    assert(chunk_index >= 0);
    /* calculate corresponding bitmap index */
    int bitmap_index = chunk_index / BITS_PER_BYTE;
    assert(bitmap_index < bitmap_size);
    int bitmap_offset = chunk_index % BITS_PER_BYTE;
    /* set this bitmap entry to 0 */
    unsigned char updated_byte = bitmap[bitmap_index] & (unsigned char) (~(1 << bitmap_offset));
    bitmap[bitmap_index] = updated_byte;
    return;
}

int num_used_chunks()
{
    int i = 0;
    int used_chunks = 0;

    while (i < bitmap_size)
    {
        unsigned char x = bitmap[i];
        if (x != 0)
        {
            /* some used chunks here */
            unsigned char j;
            for (j = 1; j <= x; j = j << 1)
            {
                if (x & j)
                {
                    used_chunks++;
                }
            }
        }
        i++;
    }
    return used_chunks;
}
