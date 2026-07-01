#ifndef ARCH_VM_H
#define ARCH_VM_H

#include <kernel/error.h>
#include <stddef.h>
#include <x86/cpu/paging.h>
#include <x86/memory/vm.h>

#define ARCH_VM_PAGE_SIZE (PAGE_SIZE)
#define ARCH_VM_TABLE_SIZE (sizeof(page_table_t))

typedef struct x86_vm_map arch_vm_map_t;

static inline void *arch_vm_lowmap_ptr(uintptr_t paddr) {
    return x86_vm_lowmap_ptr(paddr);
}

static inline void arch_vm_map_switch(arch_vm_map_t *mapping) {
    page_directory_load(mapping->page_directory_paddr);
    page_directory_reload();
}

static inline kerror_t arch_vm_tmp_map(uintptr_t paddr, size_t size,
                                       void **out_ptr) {
    kerror_t err = x86_vm_tmp_map(paddr, PAGE_NUMBER_UP(size));
    if (IS_ERROR(err)) {
        *out_ptr = NULL;
    } else {
        *out_ptr = (void *)(X86_VM_TMP_PAGE_VADDR);
    }
    return err;
}

static inline kerror_t arch_vm_tmp_unmap(void) { return x86_vm_tmp_unmap(); }

static inline void arch_vm_map_init(arch_vm_map_t *map, uintptr_t map_paddr) {
    x86_vm_map_init(map, map_paddr);
}

static inline void arch_vm_map_table_map(struct x86_vm_map *map,
                                         uintptr_t vaddr,
                                         uintptr_t table_paddr) {
    x86_vm_map_table_map(map, vaddr, table_paddr);
}
static inline uintptr_t arch_vm_map_table_unmap(struct x86_vm_map *map,
                                                uintptr_t vaddr) {
    return x86_vm_map_table_unmap(map, vaddr);
}
static inline kerror_t arch_vm_map_mem_map(struct x86_vm_map *map,
                                           uintptr_t vaddr, uintptr_t paddr) {
    return x86_vm_map_mem_map(map, vaddr, paddr);
}
static inline uintptr_t arch_vm_map_mem_unmap(struct x86_vm_map *map,
                                              uintptr_t vaddr) {
    return x86_vm_map_mem_unmap(map, vaddr);
}

#endif
