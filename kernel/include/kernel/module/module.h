#ifndef KERNEL_MODULE_H
#define KERNEL_MODULE_H

#include <kernel/boot.h>
#include <kernel/memory/frame.h>
#include <kernel/process/process.h>
#include <kernel/process/thread.h>

void module_load(struct boot_module *module, struct process *process,
                 struct thread *thread, struct memory_frame *frame,
                 size_t arg_size, void *arg);

#endif
