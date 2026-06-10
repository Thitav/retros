#ifndef KERNEL_ELF_H
#define KERNEL_ELF_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/memory/vm.h>
#include <kernel/memory/pm.h>

void *elf32_load(uintptr_t paddr);

#endif
