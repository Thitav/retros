#include <kernel/process/process.h>
#include <kernel/capability/capability.h>
#include <kernel/memory/vm.h>
#include <stddef.h>
#include <kernel/lib/string.h>
#include <kernel/process/scheduler.h>

void process_init(struct process *process) {
    memset(process, 0, sizeof(struct process));
    vm_mapping_init(process->vm_mapping);
    capability_node_init(&process->capability_node);
}
