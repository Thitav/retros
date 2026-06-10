#ifndef ARCH_VM_H
#define ARCH_VM_H

#include <x86/memory/vm.h>
#include <x86/cpu/paging.h>

#define ARCH_IMPL_VM_PAGE_SIZE PAGE_SIZE

typedef struct x86_vm_mapping arch_vm_mapping_t;

static inline void arch_vm_mapping_switch(arch_vm_mapping_t *mapping) {
    page_directory_load(mapping->page_directory_paddr);
}

void arch_vm_mapping_init(struct x86_vm_mapping *mapping);
void arch_vm_mapping_mmap(struct x86_vm_mapping *mapping, uintptr_t vaddr, uintptr_t paddr);
uintptr_t arch_vm_mapping_munmap(struct x86_vm_mapping *mapping, uintptr_t vaddr);

#endif
