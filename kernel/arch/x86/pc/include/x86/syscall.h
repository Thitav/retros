#ifndef X86_SYSCALL_H
#define X86_SYSCALL_H

#include <kernel/types/syscall.h>
#include <x86/cpu/isr.h>

void x86_syscall_set_handler(syscall_handler_t handler);
struct isr_context *x86_syscall_handler(struct isr_context *context);

#endif
