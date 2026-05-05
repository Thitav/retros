#include <kernel/arch/vmm.h>
#include <kernel/memory/pmm.h>
#include <stddef.h>
#include <stdint.h>

struct vmm_kernel_freelist_node {
    uintptr_t base;
    size_t size;
    struct vmm_kernel_freelist_node *next;
};

static struct vmm_kernel_freelist_node kernel_freelist_head = {.base = 0xABC, .size = 0xABC, .next = NULL};

void *vmm_kernel_mmap(size_t size) {
    size_t pages = (size + ARCH_VMM_PAGE_SIZE - 1) / ARCH_VMM_PAGE_SIZE;
    if (pages > pmm_available_pages()) {
        return NULL;
    }

    size = pages * ARCH_VMM_PAGE_SIZE;

    struct vmm_kernel_freelist_node *node = &kernel_freelist_head;
    while (node != NULL && node->size < size)
    {
        node = node->next;
    }
    // didnt find free
    if (node == NULL) {
        return NULL;
    }

    node->size -= size;
    uintptr_t vaddr = node->base + node->size;
    
    for (size_t i = 0; i < pages; i++) {
        arch_vmm_map(vaddr + (i * ARCH_VMM_PAGE_SIZE), pmm_alloc());
    }

    return (void *)vaddr;
}

void vmm_kernel_munmap(void *vptr, size_t size) {
    size_t pages = (size + ARCH_VMM_PAGE_SIZE - 1) / ARCH_VMM_PAGE_SIZE;
    size = pages * ARCH_VMM_PAGE_SIZE;

    uintptr_t vaddr = (uintptr_t)vptr;
    for (size_t i = 0; i < pages; i++) {
        pmm_free(arch_vmm_unmap(vaddr + (i * ARCH_VMM_PAGE_SIZE)));
    }

    struct vmm_kernel_freelist_node *node = &kernel_freelist_head;
    struct vmm_kernel_freelist_node *next_node = node->next;
    do {
        if (vaddr < next_node->base) {
            
        }
    } while (next_node != NULL);
}
