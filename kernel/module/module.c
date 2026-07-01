#include "elf.h"
#include "kernel/error.h"
#include "kernel/memory/vm.h"
#include <kernel/boot.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/memory/frame.h>
#include <kernel/module/module.h>

void module_load(struct boot_module *module, struct process *process,
                 struct thread *thread, struct memory_frame *frame,
                 size_t arg_size, void *arg) {
    vm_map_switch(&process->vm_map);

    void *module_ptr;
    ASSERT_SUCCESS(vm_tmp_map(module->paddr, module->size, &module_ptr),
                   panic("module loader failed to tmp map module"));

    struct memory_frame *page_frame =
        vm_lowmap_ptr(memory_frame_alloc(frame, sizeof(struct memory_frame)));
    memory_frame_split(frame, page_frame, VM_PAGE_SIZE);

    memory_frame_map(page_frame);
    capability_node_mint(
        process_capability_node(process), CAPABILITY_TYPE_MAPPED_FRAME,
        CAPABILITY_PERMISSION_READ | CAPABILITY_PERMISSION_WRITE, page_frame,
        NULL);

    void *entry = elf32_load(module_ptr, frame, process);
    ASSERT_SUCCESS(vm_tmp_unmap(),
                   panic("module loader failed to tmp unmap module"));

    uintptr_t ustack_table_paddr = memory_frame_alloc(frame, VM_TABLE_SIZE);
    capability_node_mint(
        process_capability_node(process), CAPABILITY_TYPE_VM_TABLE,
        CAPABILITY_PERMISSION_READ | CAPABILITY_PERMISSION_WRITE,
        vm_lowmap_ptr(ustack_table_paddr), NULL);

    thread_init(thread, page_frame->paddr, ustack_table_paddr, process,
                (thread_entry_t)entry, arg_size, arg);
}
