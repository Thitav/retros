#ifndef _KERNEL_MEMORY_SLAB_H
#define _KERNEL_MEMORY_SLAB_H

#include <stddef.h>

enum slab_size {
    SLAB_SLOT_SIZE_32   = 32,
    SLAB_SLOT_SIZE_64   = 64,
    SLAB_SLOT_SIZE_128  = 128,
    SLAB_SLOT_SIZE_256  = 256,
    SLAB_SLOT_SIZE_512  = 512,
    SLAB_SLOT_SIZE_1024 = 1024,
    
    SLAB_MIN_SLOT_SIZE = SLAB_SLOT_SIZE_32,
    SLAB_MAX_SLOT_SIZE = SLAB_SLOT_SIZE_1024,
    SLAB_BUCKETS_COUNT = 6
};

void *slab_malloc(size_t size);
size_t slab_free(void *ptr);

#endif
