#include <kernel/capability/capability.h>
#include <kernel/lib/string.h>
#include <kernel/memory/vm.h>
#include <kernel/process/process.h>
#include <kernel/process/scheduler.h>
#include <stddef.h>

void process_init(struct process *process, uintptr_t vm_map_paddr) {
    memset(process, 0, sizeof(struct process));
    vm_map_init(&process->vm_map, vm_map_paddr);
    capability_node_init(&process->capability_node);
}
