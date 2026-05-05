#include <kernel/font.h>
#include <kernel/tty.h>
#include <kernel/arch/timer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel/process/scheduler.h>
#include <kernel/boot.h>
#include <kernel/memory/pmm.h>

void thread1(void) {
    printf("1\n");
}

void thread2(void) {
    printf("2\n");
}

void thread3(void) {
    printf("3\n");
}

void kernel_main(struct boot_info *boot_info)
{
    vga_init(boot_info->framebuffer_width, boot_info->framebuffer_height);
    tty_init(boot_info->framebuffer_width, boot_info->framebuffer_height, VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK, &font_univga);

    pmm_init(boot_info->memory_maps, boot_info->memory_maps_length);

    pmm_alloc();
    char s[16];
    ltoa(pmm_alloc(), &s[0], 16);
    printf("%s\n", s);

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
