#ifndef X86_MEMORY_PM
#define X86_MEMORY_PM

#include <kernel/utils/bitmap.h>
#include <x86/cpu/paging.h>
#include <stdint.h>

#define X86_PM_PAGE_TO_BITMAP_INDEX(page) ((page) / BITMAP_BITS_PER_ELEMENT)
#define X86_PM_PAGE_TO_BITMAP_OFFSET(page) ((page) % BITMAP_BITS_PER_ELEMENT)

#define X86_PM_PAGES_COUNT (UINTPTR_MAX / PAGE_SIZE)
#define X86_PM_USED_PAGES_LEN BITMAP_ELEMENTS(X86_PM_PAGES_COUNT)

static bitmap_t x86_pm_used[X86_PM_USED_PAGES_LEN] = { 0 };

static inline void x86_pm_set_available(size_t page) {
    bitmap_clear(&x86_pm_used, X86_PM_PAGE_TO_BITMAP_OFFSET(page));
}

static inline void x86_pm_set_reserved(size_t page) {
    bitmap_set(&x86_pm_used, X86_PM_PAGE_TO_BITMAP_OFFSET(page));
}

uintptr_t x86_pm_alloc(size_t pages);

#endif
