#include <x86/thread.h>
#include <x86/cpu/paging.h>
#include <x86/memory/pm.h>
#include <x86/memory/vm.h>
// [TODO] make this internal
#include <arch/vm.h>
#include <arch/pm.h>
#include <kernel/lib/stdio.h>

extern void x86_thread_trampoline();

struct x86_thread_init_stack_frame {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip; // will point to x86_thread_trampoline

    uint32_t entry; // thread entry function
    uint32_t argc;  // thread entry function argument
    uint32_t argv;
};

void x86_thread_init(struct x86_thread_context *context, struct x86_vm_mapping *mapping, thread_entry_t entry, int argc, char **argv) {
    uintptr_t stack_vaddr = KERNEL_BASE - (PAGE_SIZE * 2);
    uintptr_t stack_paddr = x86_pm_alloc(1);
    arch_vm_mapping_mmap(mapping, stack_vaddr, stack_paddr);

    uint32_t real_esp = (uint32_t)arch_pm_kmap_vaddr(stack_paddr + PAGE_SIZE);
    uint32_t virt_esp = stack_vaddr + PAGE_SIZE;

    real_esp -= sizeof(struct x86_thread_init_stack_frame);
    virt_esp -= sizeof(struct x86_thread_init_stack_frame);

    struct x86_thread_init_stack_frame *frame = (struct x86_thread_init_stack_frame *)real_esp;
    frame->edi = 0;
    frame->esi = 0;
    frame->ebx = 0;
    frame->ebp = 0;
    frame->eip = (uint32_t)x86_thread_trampoline;
    frame->entry = (uint32_t)entry;
    frame->argc = (uint32_t)argc;
    frame->argv = (uint32_t)argv;

    context->esp = virt_esp;
}
