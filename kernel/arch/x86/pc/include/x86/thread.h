#ifndef X86_THREAD_H
#define X86_THREAD_H

#include <kernel/types/thread.h>
#include <stdint.h>
#include <x86/memory/vm.h>

struct x86_thread_context {
    uint32_t esp;
};

void x86_thread_init(struct x86_thread_context *context, struct x86_vm_mapping *mapping, thread_entry_t entry, int argc, char **argv);
extern void x86_thread_context_switch(uint32_t *current_esp, uint32_t new_esp);
extern void x86_thread_context_jump(uint32_t new_esp);

#endif
