#ifndef I386_THREAD_H
#define I386_THREAD_H

#include <stdint.h>

struct thread_context {
    uintptr_t kernel_stack_ptr;
};

extern void _thread_context_switch_regs(void **old_sp, void *new_sp);

static inline void thread_context_switch(struct thread_context *current, struct thread_context *next) {
    _thread_context_switch_regs(&current->kernel_stack_ptr, next->kernel_stack_ptr);
}

#endif
