#include <kernel/process/process.h>
#include <kernel/arch/vmm.h>
#include <stddef.h>

void process_init(struct process *process, struct thread_new *main_thread) {
    process->vmm_mapping_paddr = arch_vmm_mapping_new();
    process->main_thread = NULL;
}
