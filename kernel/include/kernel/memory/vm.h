#ifndef KERNEL_MEMORY_VM_H
#define KERNEL_MEMORY_VM_H

#include <arch/vm.h>
#include <stdint.h>

#define VM_PAGE_SIZE ARCH_IMPL_VM_PAGE_SIZE

typedef arch_vm_mapping_t vm_mapping_t;

static inline void vm_mapping_switch(vm_mapping_t *mapping) {
    arch_vm_mapping_switch(mapping);
}

static inline void vm_mapping_init(vm_mapping_t *mapping) {
    arch_vm_mapping_init(mapping); 
}

static inline void vm_mapping_mmap(vm_mapping_t *mapping, uintptr_t vaddr, uintptr_t paddr) {
    arch_vm_mapping_mmap(mapping, vaddr, paddr);
}

static inline uintptr_t vm_mapping_munmap(vm_mapping_t *mapping, uintptr_t vaddr) {
    return arch_vm_mapping_munmap(mapping, vaddr);
}

#endif
