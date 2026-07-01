#ifndef ARCH_SYSCALL_H
#define ARCH_SYSCALL_H

#include <x86/syscall.h>
#include <x86/cpu/isr.h>
#include <kernel/types/syscall.h>

static inline void arch_syscall_init(void) {
    // [TODO] move 0x80 to a constant
    isr_set_handler(0x80, x86_syscall_handler);
}

static inline void arch_syscall_set_handler(syscall_handler_t handler) {
    x86_syscall_set_handler(handler);
}

#endif
