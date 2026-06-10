#ifndef KERNEL_SYSPROC_H
#define KERNEL_SYSPROC_H

#include <kernel/process/thread.h>
#include <kernel/process/process.h>

void sysproc_load(uintptr_t module_paddr, struct process *sysproc_process, struct thread *sysproc_thread);

#endif
