#ifndef ARCH_PM_H
#define ARCH_PM_H

#include <x86/memory/pm.h>
#include <x86/cpu/paging.h>

#define ARCH_IMPL_PM_PAGE_SIZE PAGE_SIZE
#define ARCH_PM_KMAP_VADDR(paddr) (KERNEL_BASE + paddr)

static inline void *arch_pm_kmap_vaddr(uintptr_t paddr) {
    return (void *)ARCH_PM_KMAP_VADDR(paddr);
}

static inline uintptr_t arch_pm_alloc(size_t pages) {
    return x86_pm_alloc(pages);
} 

#endif
