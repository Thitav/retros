#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <arch/syscall/syscall.h>
#include <kernel/types/syscall.h>
#include <stddef.h>

void syscall_init(void);

static inline void syscall_set_handler(syscall_handler_t handler) {
    arch_syscall_set_handler(handler);
}

enum syscall_number {
    SYSCALL_NUMBER_IPC_SEND = 0,
    SYSCALL_NUMBER_IPC_RECEIVE = 1,
    SYSCALL_NUMBER_IPC_FUNCTION = 2,
    SYSCALL_NUMBER_IPC_CALL = 3,
    SYSCALL_NUMBER_IPC_RETURN = 4,
    SYSCALL_NUMBER_UNTYPED_FRAME_RETYPE = 5,
};

#endif
