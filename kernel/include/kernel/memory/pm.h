#ifndef KERNEL_MEMORY_PM_H
#define KERNEL_MEMORY_PM_H

#include <arch/pm.h>
#include <stdint.h>
#include <stddef.h>

#define PM_PAGE_SIZE ARCH_PM_PAGE_SIZE
#define PM_KMAP_VADDR ARCH_PM_KMAP_VADDR

static inline void *pm_kmap_vaddr(uintptr_t paddr) {
    return arch_pm_kmap_vaddr(paddr);
}

static inline uintptr_t pm_alloc(size_t pages) {
    return arch_pm_alloc(pages);
}

#endif
