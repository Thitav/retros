#ifndef KERNEL_TYPES_SYSCALL_H
#define KERNEL_TYPES_SYSCALL_H

#include <stddef.h>

typedef void (*syscall_handler_t)(struct syscall_args *);

struct syscall_args {
    size_t num;    // Syscall number
    size_t arg0;   // Mapping: ebx (x86) or a0 (RISC-V)
    size_t arg1;   // Mapping: ecx (x86) or a1 (RISC-V)
    size_t arg2;
    size_t arg3;
    size_t ret;    // Where the result goes
};

#endif
