#include <kernel/memory/vmm.h>
#include <kernel/arch/vmm.h>
#include "slab.h"

void *kmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    if (size > SLAB_MAX_SIZE) {
        size = ((size + ARCH_VMM_PAGE_SIZE - 1) / ARCH_VMM_PAGE_SIZE) * ARCH_VMM_PAGE_SIZE;
        return vmm_kernel_mmap(size);
    } else {
        return slab_alloc(size);
    }
}
