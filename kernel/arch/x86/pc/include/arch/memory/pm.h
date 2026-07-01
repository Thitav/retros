#ifndef ARCH_PM_H
#define ARCH_PM_H

#include <x86/memory/pm.h>
#include <x86/cpu/paging.h>

#define ARCH_IMPL_PM_PAGE_SIZE PAGE_SIZE

static inline uintptr_t arch_pm_alloc(size_t pages) {
    return x86_pm_alloc(pages);
} 

#endif
