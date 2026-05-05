#ifndef I386_VMM_H
#define I386_VMM_H

#include <i386/cpu/paging.h>

#define ARCH_IMPL_VMM_PAGE_SIZE PAGE_SIZE

typedef page_directory_t arch_impl_vmm_mapping_t;

static inline void arch_impl_vmm_switch(arch_impl_vmm_mapping_t new_mapping) {
    page_directory_load(new_mapping);
}

uintptr_t arch_impl_vmm_mapping_new(void);
void arch_impl_vmm_map(uintptr_t vaddr, uintptr_t paddr);
uintptr_t arch_impl_vmm_unmap(uintptr_t vaddr);

#endif
