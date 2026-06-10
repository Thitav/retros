#include "kheap.h"
#include <kernel/memory/vm.h>
#include <limits.h>

#define KHEAP_BASE 0xC0400000
#define KHEAP_SIZE 0x2800000
#define KHEAP_END (KHEAP_BASE + KHEAP_SIZE)
#define KHEAP_PAGES (KHEAP_SIZE / VM_PAGE_SIZE)
#define KHEAP_MAX_ALLOC_SIZE (UCHAR_MAX * VM_PAGE_SIZE)

static unsigned char kheap_alloc_map[KHEAP_PAGES] = { 0 };

void *kheap_malloc(size_t size) {
    if (size > KHEAP_MAX_ALLOC_SIZE) {
        return NULL;
    }

    unsigned char pages = (size + VM_PAGE_SIZE - 1) / VM_PAGE_SIZE;

    size_t current_page = 0;
    unsigned char free_count = 0;
    while (current_page < KHEAP_PAGES && free_count < pages) {
        unsigned char alloc = kheap_alloc_map[current_page + free_count];
        if (alloc > 0) {
            current_page += free_count + alloc;
            free_count = 0;
        } else {
            free_count++;
        }
    }
    if (free_count < pages) {
        return NULL;
    }

    kheap_alloc_map[current_page] = free_count;
    uintptr_t addr = KHEAP_BASE + (current_page * VM_PAGE_SIZE);
    vm_mmap(addr, free_count);
    return (void *)addr;
}

void kheap_free(void *ptr) {
    size_t page = ((uintptr_t)ptr - KHEAP_BASE) / VM_PAGE_SIZE;
    vm_munmap((uintptr_t)ptr, kheap_alloc_map[page]);
    kheap_alloc_map[page] = 0;
}
