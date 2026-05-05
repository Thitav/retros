#ifndef KERNEL_MEMORY_VMM_H
#define KERNEL_MEMORY_VMM_H

#include <stddef.h>

void *vmm_kernel_mmap(size_t size);

#endif
