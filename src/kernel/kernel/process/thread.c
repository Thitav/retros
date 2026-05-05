#include <kernel/arch/vmm.h>
#include <kernel/arch/thread.h>
#include <kernel/process/process.h>
#include <kernel/process/thread.h>

void thread_init(struct thread_new *thread);

void thread_switch(struct thread_new *current, struct thread_new *next) {
    if (current == next) {
        return;
    }

    if (current->process->vmm_mapping_paddr != next->process->vmm_mapping_paddr) {
        arch_vmm_switch(next->process->vmm_mapping_paddr);
    }

    arch_thread_context_switch(&current->context, &next->context);
}
