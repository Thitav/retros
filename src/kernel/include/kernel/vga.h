#ifndef KERNEL_VGA_H
#define KERNEL_VGA_H

#include <stdint.h>
#include <kernel/font.h>

static uint8_t *const VGA_MEMORY = (uint8_t *)0xA0000;

struct vga_screen
{
  uint32_t width, height;
};
enum vga_color
{
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

void vga_init(uint32_t width, uint32_t height);
void vga_putpixel(uint32_t pos_x, uint32_t pos_y, enum vga_color color);
void vga_fillrect(uint32_t width, uint32_t height, enum vga_color color);
void vga_drawchar(const struct bitmap_font *font, char ch,
                  uint32_t pos_x, uint32_t pos_y, enum vga_color color_fg, enum vga_color color_bg);

#endif
