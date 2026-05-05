#ifndef _KERNEL_MEMORY_SLAB_H
#define _KERNEL_MEMORY_SLAB_H

#include <stddef.h>

enum slab_size {
    SLAB_SIZE_32   = 32,
    SLAB_SIZE_64   = 64,
    SLAB_SIZE_128  = 128,
    SLAB_SIZE_256  = 256,
    SLAB_SIZE_512  = 512,
    SLAB_SIZE_1024 = 1024,
    
    SLAB_MIN_SIZE = SLAB_SIZE_32,
    SLAB_MAX_SIZE = SLAB_SIZE_1024,
    SLAB_BUCKETS_COUNT = 6
};

void *slab_alloc(size_t size);
void slab_free(void *addr);

#endif
