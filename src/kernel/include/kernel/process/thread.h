#ifndef KERNEL_THREAD_H
#define KERNEL_THREAD_H

#include <kernel/arch/thread.h>

struct process;

struct thread_new {
    arch_thread_context_t context;
    struct process *process;
};

#endif
