#include <kernel/syscall.h>
#include "ipc.h"
#include "frame.h"
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>

static void _test_syscall(struct syscall_args *args) {
    char s[16];
    lutoa(args->arg0, s, 10);
    printf(" test syscall %s ", s);
}

static const syscall_handler_t syscall_table[] = {
    [SYSCALL_NUMBER_IPC_SEND] = syscall_ipc_send,
    [SYSCALL_NUMBER_IPC_RECEIVE] = syscall_ipc_receive,
    [SYSCALL_NUMBER_IPC_FUNCTION] = syscall_ipc_function,
    [SYSCALL_NUMBER_IPC_CALL] = syscall_ipc_call,
    [SYSCALL_NUMBER_IPC_RETURN] = syscall_ipc_return,
    [SYSCALL_NUMBER_UNTYPED_FRAME_RETYPE] = syscall_untyped_frame_retype,
    [69] = _test_syscall,
};

void syscall_handler(struct syscall_args *args) {
    syscall_table[args->num](args);
}

void syscall_init(void) {
    arch_syscall_init();
    arch_syscall_set_handler(syscall_handler);
}
