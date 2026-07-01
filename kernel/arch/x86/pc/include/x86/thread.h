#ifndef X86_THREAD_H
#define X86_THREAD_H

#include <kernel/types/thread.h>
#include <stddef.h>
#include <stdint.h>
#include <x86/cpu/paging.h>
#include <x86/memory/vm.h>

struct x86_thread_context {
    uint32_t esp;
    uint32_t kernel_esp;
    char kstack[PAGE_SIZE * 2];
};

void x86_thread_init(struct x86_thread_context *context,
                     uintptr_t ustack_base_paddr, uintptr_t ustack_table_paddr,
                     thread_entry_t entry, size_t arg_size, void *arg);
extern void
x86_thread_context_switch(struct x86_thread_context *current_context,
                          struct x86_thread_context *next_context);
extern void x86_thread_context_jump(struct x86_thread_context *context);

#endif
