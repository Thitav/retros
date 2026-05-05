#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <stdint.h>

struct thread_new;

struct process {
    uintptr_t vmm_mapping_paddr;
    struct thread_new *main_thread;
};

#endif
