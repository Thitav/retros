#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>
#include <x86/cpu/paging.h>
#include <x86/kernel/kstack.h>
#include <x86/memory/pm.h>
#include <x86/memory/vm.h>
#include <x86/thread.h>
#include <x86/vas.h>

extern void x86_thread_trampoline();

struct x86_thread_init_stack_frame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip; // will point to x86_thread_trampoline

    uint32_t entry; // thread entry function
    uint32_t ret_addr;
    uint32_t arg_size; // thread entry function argument
    uint32_t arg;
};

void x86_thread_init(struct x86_thread_context *context,
                     uintptr_t ustack_base_paddr, uintptr_t ustack_table_paddr,
                     thread_entry_t entry, size_t arg_size, void *arg) {
    uintptr_t ustack_vaddr = X86_VAS_KERNEL_BASE - PAGE_SIZE;
    // uintptr_t args_paddr = x86_pm_alloc(1);
    x86_vm_map_table_map(NULL, ustack_vaddr, ustack_table_paddr);
    x86_vm_map_mem_map(NULL, ustack_vaddr, ustack_base_paddr);

    uintptr_t esp = ALIGN_DOWN(ustack_vaddr + PAGE_SIZE - arg_size, 16);
    memcpy((void *)esp, arg, arg_size);
    uintptr_t args_base = esp;

    esp -= sizeof(struct x86_thread_init_stack_frame);
    struct x86_thread_init_stack_frame *frame =
        (struct x86_thread_init_stack_frame *)esp;
    frame->edi = 0;
    frame->esi = 0;
    frame->ebx = 0;
    frame->ebp = 0;
    frame->eip = (uint32_t)x86_thread_trampoline;
    frame->entry = (uint32_t)entry;
    frame->ret_addr = 0; // [TODO] return to something like cleanup
    frame->arg_size = (uint32_t)arg_size;
    frame->arg = (uint32_t)args_base;

    context->esp = esp;
    context->kernel_esp =
        (uint32_t)&context->kstack[0] + sizeof(context->kstack);
}
