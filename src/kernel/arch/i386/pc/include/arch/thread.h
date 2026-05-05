#ifndef I386_PC_THREAD_H
#define I386_PC_THREAD_H

#include <i386/thread.h>
#include <stdint.h>

typedef struct thread_context arch_impl_thread_context_t;

static inline void arch_impl_thread_context_switch(arch_impl_thread_context_t *current, arch_impl_thread_context_t *next) {
    thread_context_switch(current, next);
}

#endif
