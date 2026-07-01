#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <kernel/capability/capability.h>
#include <kernel/memory/vm.h>
#include <stddef.h>
#include <stdint.h>

struct process {
    vm_map_t vm_map;
    struct capability_node capability_node;
};

void process_init(struct process *process, uintptr_t vm_map_paddr);

static inline struct capability *process_capability(struct process *process,
                                                    size_t index) {
    return capability_node_get(&process->capability_node, index);
}

static inline struct capability_node *
process_capability_node(struct process *process) {
    return &process->capability_node;
}

#endif
