#ifndef KERNEL_ARCH_THREAD_H
#define KERNEL_ARCH_THREAD_H

#include <arch/thread.h>

typedef arch_impl_thread_context_t arch_thread_context_t;

static inline void arch_thread_context_switch(arch_thread_context_t *current, arch_thread_context_t *next) {
    arch_impl_thread_context_switch(current, next);
}

#endif
