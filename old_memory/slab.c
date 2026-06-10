#include <stddef.h>
#include <kernel/memory/vm.h>
#include <stdint.h>
#include <stdbool.h>
#include "slab.h"
#include "kheap.h"
#include <kernel/lib/stdio.h>

#define SLAB_SIZE VM_PAGE_SIZE
#define SLAB_HEADER_MAGIC 0xDEADBEEF

struct slab_header {
    unsigned int magic;

    size_t slot_size;
    char *next_slot_free;
    char *next_slot_alloc;

    struct slab_header *next_header;
    struct slab_header *prev_header;
};

struct slab_bucket {
    struct slab_header *partial_head;
    struct slab_header *full_head;
};

static struct slab_bucket slab_buckets[SLAB_BUCKETS_COUNT] = { NULL };

// [TODO] optimize this
static inline unsigned char slab_bucket_index(size_t size) {
    if (size <= 32)   return 0;
    if (size <= 64)   return 1;
    if (size <= 128)  return 2;
    if (size <= 256)  return 3;
    if (size <= 512)  return 4;
    if (size <= 1024) return 5;
    return 255;
    // return -1; // Larger than max slab
}

static inline size_t slab_slot_size(size_t size) {
    if (size <= 32)   return 32;
    if (size <= 64)   return 64;
    if (size <= 128)  return 128;
    if (size <= 256)  return 256;
    if (size <= 512)  return 512;
    if (size <= 1024) return 1024;
    return 0;
}

static inline void header_partial_to_full(struct slab_bucket *bucket, struct slab_header *header) {
    if (header->prev_header != NULL) {
        header->prev_header->next_header = header->next_header; 
    } else {
        bucket->partial_head = header->next_header;
    }
    
    if (header->next_header != NULL) {
        header->next_header->prev_header = header->prev_header;
    }

    header->next_header = bucket->full_head;
    header->prev_header = NULL;

    if (bucket->full_head != NULL) {
        bucket->full_head->prev_header = header;
    }
    bucket->full_head = header;
}

static inline void header_full_to_partial(struct slab_bucket *bucket, struct slab_header *header) {
    if (header->prev_header != NULL) {
        header->prev_header->next_header = header->next_header; 
    } else {
        bucket->full_head = header->next_header;
    }
    
    if (header->next_header != NULL) {
        header->next_header->prev_header = header->prev_header;
    }

    header->next_header = bucket->partial_head;
    header->prev_header = NULL;

    if (bucket->partial_head != NULL) {
        bucket->partial_head->prev_header = header;
    }
    bucket->partial_head = header;
}

void *slab_malloc(size_t size) {
    size = slab_slot_size(size);
    struct slab_bucket *bucket = &slab_buckets[slab_bucket_index(size)];

    struct slab_header *header = NULL;
    if (bucket->partial_head != NULL) {
        header = bucket->partial_head;
    } else {
        header = (struct slab_header *)kheap_malloc(SLAB_SIZE);
        header->magic = SLAB_HEADER_MAGIC;
        header->slot_size = size;
        header->next_header = NULL;
        header->next_slot_free = NULL;
        header->next_slot_alloc = (char *)((((uintptr_t)(header + 1) + size - 1) / size) * size);
        bucket->partial_head = header;
    }

    char *allocated_slot = NULL;

    if (header->next_slot_free != NULL) {
        allocated_slot = header->next_slot_free;
        header->next_slot_free = (char *)*header->next_slot_free;
    } else {
        allocated_slot = header->next_slot_alloc;
        header->next_slot_alloc += size;
        if (((uintptr_t)header->next_slot_alloc - (uintptr_t)header) / SLAB_SIZE > 0) {
            header->next_slot_alloc = NULL;
            header_partial_to_full(bucket, header);
        }
    }

    return (void *)allocated_slot;
}

size_t slab_free(void *ptr) {
    struct slab_header *header = (struct slab_header *)(((uintptr_t)ptr / SLAB_SIZE) * SLAB_SIZE);
    if (header->magic != SLAB_HEADER_MAGIC) {
        return 0;
    }

    if (header->next_slot_free == NULL && header->next_slot_alloc == NULL) {
        struct slab_bucket *bucket = &slab_buckets[slab_bucket_index(header->slot_size)];
        header_full_to_partial(bucket, header);
    }

    *((char **)ptr) = (char *)header->next_slot_free;
    header->next_slot_free = (char *)ptr;

    return header->slot_size;
}
