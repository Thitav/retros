#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/utils/helpers.h>

kerror_t helper_vm_mem_map(vm_map_t *map, uintptr_t vaddr, uintptr_t paddr,
                           struct memory_frame *frame) {
    kerror_t err = vm_map_mem_map(map, vaddr, paddr);

    if (err == KERROR_VM_MISSING_TABLE) {
        uintptr_t table_paddr = memory_frame_alloc(frame, VM_TABLE_SIZE);
        vm_map_table_map(map, vaddr, table_paddr);
        err = vm_map_mem_map(map, vaddr, paddr);
    }
    return err;
}
