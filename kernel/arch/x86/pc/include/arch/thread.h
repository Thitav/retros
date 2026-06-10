#ifndef ARCH_THREAD_H
#define ARCH_THREAD_H

#include <x86/thread.h>
#include <arch/interrupt.h>
#include <arch/vm.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>

typedef struct x86_thread_context arch_thread_context_t;

static inline void arch_thread_context_switch(arch_thread_context_t *current, arch_thread_context_t *next) {
    if (current == NULL) {
        x86_thread_context_jump(next->esp);
    } else {
        x86_thread_context_switch(&current->esp, next->esp);
    }
}

static inline void arch_thread_init(arch_thread_context_t *context, arch_vm_mapping_t *mapping, thread_entry_t entry, int argc, char **argv) {
    x86_thread_init(context, mapping, entry, argc, argv);
}

#endif
