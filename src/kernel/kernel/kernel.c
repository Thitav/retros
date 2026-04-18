#include <kernel/font.h>
#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <kernel/arch/timer.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "scheduler.h"

void thread1(void) {
    printf("1\n");
}

void thread2(void) {
    printf("2\n");
}

void thread3(void) {
    printf("3\n");
}

void kernel_entry(uint32_t mb_magic, multiboot_info_t *mb_info)
{
    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        // Should be kernel panic
        return;
    }

    vga_init(mb_info->framebuffer_width, mb_info->framebuffer_height);
    tty_init(mb_info->framebuffer_width, mb_info->framebuffer_height, VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK, &font_univga);

    printf("Hello from kernel!\n");
    printf("Have a nice day!\n");

    struct thread t1 = {.target = thread1};
    struct thread t2 = {.target = thread2};
    struct thread t3 = {.target = thread3};

    scheduler_add(&t1);
    scheduler_add(&t2);
    scheduler_add(&t3);

    scheduler_init();

    while (1) {}
}
