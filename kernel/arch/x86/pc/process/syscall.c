#include <x86/syscall.h>
#include <x86/cpu/isr.h>
#include <stddef.h>

syscall_handler_t kernel_syscall_handler = NULL;

void x86_syscall_set_handler(syscall_handler_t handler) {
    kernel_syscall_handler = handler;
}

struct isr_context *x86_syscall_handler(struct isr_context *context) {
    struct syscall_args args;

    args.num  = context->eax;
    args.arg0 = context->ebx;
    args.arg1 = context->ecx;
    args.arg2 = context->edx;
    args.arg3 = context->esi;

    // Call the generic kernel dispatcher
    kernel_syscall_handler(&args);

    // Put the result back into the return register
    context->eax = args.ret;
    return context;
}
