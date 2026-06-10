#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <stdint.h>
#include <kernel/capability/capability.h>
#include <stddef.h>
#include <kernel/memory/vm.h>

struct process {
    vm_mapping_t *vm_mapping;
    struct capability_node capability_node;
};

void process_init(struct process *process);

static inline struct capability* process_capability(struct process *process, size_t index) {
    return capability_node_get(&process->capability_node, index);
}

static inline struct capability_node* process_capability_node(struct process *process) {
    return &process->capability_node;
}

#endif
