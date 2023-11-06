#define maxCompartments 5

typedef struct _heap_compartment
{
    void *buffer;
    size_t bytes_allocated;
    size_t max_allocated;
    void *__capability datacap;
    void *__capability identifier; // NOTE: THIS IS SEALED
} heap_compartment;

void *__capability init_compartment(size_t size_in_bytes, void *__capability dc);
void *__capability malloc_compartment(size_t len, void *__capability component_id);
void free_compartment(void *__capability compartment_id);
