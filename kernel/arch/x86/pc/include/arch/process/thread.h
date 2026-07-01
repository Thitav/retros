#ifndef ARCH_THREAD_H
#define ARCH_THREAD_H

#include "x86/cpu/paging.h"
#include <arch/interrupt.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/memory/frame.h>
#include <stddef.h>
#include <x86/memory/vm.h>
#include <x86/thread.h>

typedef struct x86_thread_context arch_thread_context_t;

static inline void arch_thread_context_switch(arch_thread_context_t *current,
                                              arch_thread_context_t *next) {
    if (current == NULL) {
        x86_thread_context_jump(next);
    } else {
        x86_thread_context_switch(current, next);
    }
}

static inline void arch_thread_init(arch_thread_context_t *context,
                                    uintptr_t ustack_base_paddr,
                                    uintptr_t ustack_table_paddr,
                                    thread_entry_t entry, size_t arg_size,
                                    void *arg) {
    x86_thread_init(context, ustack_base_paddr, ustack_table_paddr, entry,
                    arg_size, arg);
}

#endif
