#ifndef KERNEL_ELF_H
#define KERNEL_ELF_H

#include <kernel/memory/frame.h>
#include <kernel/process/process.h>

void *elf32_load(void *base, struct memory_frame *frame,
                 struct process *process);

#endif
