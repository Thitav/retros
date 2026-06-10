#ifndef KERNEL_TYPES_VM_H
#define KERNEL_TYPES_VM_H

enum vm_mmap_protection {
    VM_MMAP_PROTECTION_READ = (1 << 0),
    VM_MMAP_PROTECTION_WRITE = (1 << 1),
    VM_MMAP_PROTECTION_EXEC = (1 << 2),
};

#endif
