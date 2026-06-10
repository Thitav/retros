#include <x86/memory/pm.h>
#include <stdbool.h>

uintptr_t x86_pm_alloc(size_t pages) {
    unsigned char free_count = 0;

    size_t current_page = 0;
    while (current_page < X86_PM_PAGES_COUNT && free_count < pages) {
        bool alloc = bitmap_get(&x86_pm_used, current_page + free_count);
        if (alloc) {
            current_page += free_count + alloc;
            free_count = 0;
        } else {
            free_count++;
        }
    }
    if (free_count < pages) {
        return UINTPTR_MAX;
    }

    for (size_t i = current_page; i < current_page + free_count; i++) {
        bitmap_set(&x86_pm_used, i);
    }

    return current_page * PAGE_SIZE;
}
