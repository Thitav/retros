#ifndef x86_MEMORY_VM_H
#define x86_MEMORY_VM_H

#include <kernel/error.h>
#include <stddef.h>
#include <stdint.h>

struct x86_vm_map {
    uintptr_t page_directory_paddr;
};

#define X86_VM_TMP_PAGE_PDE_INDEX 1022
#define X86_VM_TMP_PAGE_VADDR PAGE_PD_INDEX_VADDR(X86_VM_TMP_PAGE_PDE_INDEX)

void x86_vm_lowmap_init(uintptr_t paddr, size_t size);
void *x86_vm_lowmap_ptr(uintptr_t paddr);
kerror_t x86_vm_tmp_map(uintptr_t paddr, size_t pages);
kerror_t x86_vm_tmp_unmap(void);
void x86_vm_map_init(struct x86_vm_map *map, uintptr_t pd_paddr);
void x86_vm_map_table_map(struct x86_vm_map *map, uintptr_t vaddr,
                          uintptr_t table_paddr);
uintptr_t x86_vm_map_table_unmap(struct x86_vm_map *map, uintptr_t vaddr);
kerror_t x86_vm_map_mem_map(struct x86_vm_map *map, uintptr_t vaddr,
                            uintptr_t paddr);
uintptr_t x86_vm_map_mem_unmap(struct x86_vm_map *map, uintptr_t vaddr);

#endif
