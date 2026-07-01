#include "slab.h"
#include <kernel/memory/kmalloc.h>

void *kmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    if (size > SLAB_MAX_SLOT_SIZE) {
        return kheap_malloc(size);
    }
    return slab_malloc(size);
}

void kfree(void *ptr) {
    if (slab_free(ptr) == 0) {
        kheap_free(ptr);
    }
}
