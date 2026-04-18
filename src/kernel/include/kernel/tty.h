#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/vga.h>
#include <kernel/font.h>

struct tty_terminal
{
  uint8_t color_fg, color_bg;
  uint32_t width, height;
  uint32_t cursor_x, cursor_y;
  const struct bitmap_font *font;
};

void tty_init(uint32_t width, uint32_t height, enum vga_color color_fg, enum vga_color color_bg, const struct bitmap_font *font);
void tty_putchar(char ch);
void tty_write(const char *data, size_t size);
void tty_writestr(const char *str);

#endif
