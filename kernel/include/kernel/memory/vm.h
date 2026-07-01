#ifndef KERNEL_MEMORY_VM_H
#define KERNEL_MEMORY_VM_H

#include "x86/memory/vm.h"
#include <arch/memory/vm.h>
#include <kernel/memory/frame.h>
#include <stdint.h>

#define VM_PAGE_SIZE (ARCH_VM_PAGE_SIZE)
// [TODO] rename this (table is an arch specific term?)
#define VM_TABLE_SIZE (ARCH_VM_TABLE_SIZE)

typedef arch_vm_map_t vm_map_t;

static inline void *vm_lowmap_ptr(uintptr_t paddr) {
    return x86_vm_lowmap_ptr(paddr);
}

static inline kerror_t vm_tmp_map(uintptr_t paddr, size_t size,
                                  void **out_ptr) {
    return arch_vm_tmp_map(paddr, size, out_ptr);
}

static inline kerror_t vm_tmp_unmap(void) { return arch_vm_tmp_unmap(); }

static inline void vm_map_switch(vm_map_t *map) { arch_vm_map_switch(map); }

static inline void vm_map_init(vm_map_t *map, uintptr_t map_paddr) {
    arch_vm_map_init(map, map_paddr);
}

static inline void vm_map_table_map(vm_map_t *map, uintptr_t vaddr,
                                    uintptr_t table_paddr) {
    arch_vm_map_table_map(map, vaddr, table_paddr);
}

static inline kerror_t vm_map_mem_map(vm_map_t *map, uintptr_t vaddr,
                                      uintptr_t paddr) {
    return arch_vm_map_mem_map(map, vaddr, paddr);
}

static inline uintptr_t vm_map_mem_unmap(vm_map_t *map, uintptr_t vaddr) {
    return arch_vm_map_mem_unmap(map, vaddr);
}

#endif
