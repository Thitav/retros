#ifndef KERNEL_SYSCALL_IPC_H
#define KERNEL_SYSCALL_IPC_H

#include <kernel/syscall.h>

void syscall_ipc_send(struct syscall_args *args);
void syscall_ipc_receive(struct syscall_args *args);
void syscall_ipc_function(struct syscall_args *args);
void syscall_ipc_call(struct syscall_args *args);
void syscall_ipc_return(struct syscall_args *args);

#endif
