#include <kernel/font.h>
#include <kernel/multiboot.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

    // __asm__ volatile("int $0x3");
}
