#ifndef KERNEL_ARCH_VMM_H
#define KERNEL_ARCH_VMM_H

#include <arch/vmm.h>

#define ARCH_VMM_PAGE_SIZE ARCH_IMPL_VMM_PAGE_SIZE

typedef arch_impl_vmm_mapping_t arch_vmm_mapping_t;

static inline void arch_vmm_switch(arch_vmm_mapping_t new_mapping) {
    arch_impl_vmm_switch(new_mapping);
}

static inline uintptr_t arch_vmm_mapping_new(void) {
    return arch_impl_vmm_mapping_new(); 
}

static inline void arch_vmm_map(uintptr_t vaddr, uintptr_t paddr) {
    arch_impl_vmm_map(vaddr, paddr);
}

static inline uintptr_t arch_vmm_unmap(uintptr_t vaddr) {
    return arch_impl_vmm_unmap(vaddr);
}

#endif
