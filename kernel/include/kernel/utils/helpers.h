#ifndef KERNEL_UTILS_HELPERS_H
#define KERNEL_UTILS_HELPERS_H

#include <kernel/error.h>
#include <kernel/memory/frame.h>
#include <kernel/memory/vm.h>

kerror_t helper_vm_mem_map(vm_map_t *map, uintptr_t vaddr, uintptr_t,
                           struct memory_frame *frame);

#endif
