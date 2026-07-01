#include <kernel/arch/timer.h>
#include <kernel/boot.h>
#include <kernel/font.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>
#include <kernel/memory/frame.h>
#include <kernel/module/module.h>
#include <kernel/process/process.h>
#include <kernel/process/scheduler.h>
#include <kernel/process/thread.h>
#include <kernel/syscall.h>
#include <kernel/tty.h>
#include <stdint.h>

static struct process sysproc;
static struct thread sysproc_entry;

static struct memory_frame root_frames[32] = {0};

void kernel_main(struct boot_info *boot_info) {
    vga_init(boot_info->framebuffer_width, boot_info->framebuffer_height);
    tty_init(boot_info->framebuffer_width, boot_info->framebuffer_height,
             VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK, &font_univga);

    printf("Hello from kernel!\n");
    printf("Have a nice day!\n");

    syscall_init();

    struct boot_pm_map *lowmap = &boot_info->pm_map[boot_info->lowmap_index];
    struct memory_frame *root_frame = &root_frames[0];
    memory_frame_init(root_frame, lowmap->paddr, lowmap->size);

    uintptr_t vm_map_paddr =
        memory_frame_alloc(root_frame, 1024 * sizeof(uintptr_t));
    process_init(&sysproc, vm_map_paddr);
    module_load(&boot_info->modules[0], &sysproc, &sysproc_entry, root_frame, 0,
                NULL);
    thread_switch(&sysproc_entry);

    // memory_frame_init(&root_frame, pm_alloc(32), VM_PAGE_SIZE * 32);
    // size_t cap_idx = capability_node_mint(&sysproc.capability_node,
    // CAPABILITY_TYPE_UNMAPPED_FRAME, CAPABILITY_PERMISSION_READ &
    // CAPABILITY_PERMISSION_WRITE, &root_frame);
    // module_load(&boot_info->modules[0], &sysproc, &sysproc_entry,
    // sizeof(size_t), &cap_idx); thread_switch(&sysproc_entry);

    // scheduler_init();
    // scheduler_enqueue(&sysproc_entry);

    // scheduler_start();

    while (1) {
    }
}
