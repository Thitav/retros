#include <kernel/font.h>
#include <kernel/tty.h>
#include <kernel/arch/timer.h>
#include <stdint.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>
#include <kernel/process/scheduler.h>
#include <kernel/process/thread.h>
#include <kernel/process/process.h>
#include <kernel/boot.h>
#include <kernel/sysproc/sysproc.h>
#include <kernel/syscall.h>

void kernel_main(struct boot_info *boot_info)
{
    vga_init(boot_info->framebuffer_width, boot_info->framebuffer_height);
    tty_init(boot_info->framebuffer_width, boot_info->framebuffer_height, VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK, &font_univga);

    printf("Hello from kernel!\n");
    printf("Have a nice day!\n");

    struct process sysproc = {.vm_mapping = boot_info->kernel_mapping};
    capability_node_init(&sysproc.capability_node);

    struct thread sysproc_entry;
    sysproc_load(boot_info->modules[0].paddr, &sysproc, &sysproc_entry);

    syscall_init();

    scheduler_init();
    scheduler_enqueue(&sysproc_entry);

    scheduler_start();

    while (1) {}
}
