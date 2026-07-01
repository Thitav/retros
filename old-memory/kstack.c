#include <x86/memory/pm.h> 
#include <arch/vm.h> 
#include <kernel/utils/bitmap.h>
#include <x86/kernel/kstack.h>
#include <x86/vas.h>
#include <kernel/lib/stdio.h>

#define X86_KSTACK_BITMAP_LEN BITMAP_ELEMENTS(X86_KSTACK_COUNT)
static bitmap_t x86_kstack_used[X86_KSTACK_BITMAP_LEN] = { 0 };

void x86_kstack_init(void) {
    for (size_t i = 0; i < X86_KSTACK_COUNT; i++) {
        for (unsigned char j = 0; j < X86_KSTACK_PAGES; j++) {
            uintptr_t paddr = x86_pm_alloc(1);
            arch_vm_mapping_mmap(NULL, X86_VAS_KSTACK_BASE + (i * X86_KSTACK_SIZE_WITH_GAP) + X86_KSTACK_GAP_SIZE + (j * PAGE_SIZE), paddr);
        }
    }
}

void *x86_kstack_alloc(void) {
    size_t index = bitmap_find(&x86_kstack_used, X86_KSTACK_BITMAP_LEN, 1, 1, false, NULL);
    bitmap_set(&x86_kstack_used, index);
    return (void *)(X86_VAS_KSTACK_BASE + (index * X86_KSTACK_SIZE_WITH_GAP) + X86_KSTACK_SIZE_WITH_GAP);
}
