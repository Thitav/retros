#ifndef KERNEL_SYSCALL_UNTYPED_FRAME_H
#define KERNEL_SYSCALL_UNTYPED_FRAME_H

#include <kernel/syscall.h>

void syscall_untyped_frame_retype(struct syscall_args *args);

#endif
