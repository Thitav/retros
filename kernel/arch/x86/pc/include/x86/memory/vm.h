#ifndef x86_MEMORY_VM_H
#define x86_MEMORY_VM_H

#include <stdint.h>

struct x86_vm_mapping {
    uintptr_t page_directory_paddr;
};

#endif
