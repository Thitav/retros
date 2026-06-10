#include <kernel/tty.h>

static struct tty_terminal terminal;

void tty_init(uint32_t width, uint32_t height, enum vga_color color_fg, enum vga_color color_bg, const struct bitmap_font *font)
{
  terminal.color_fg = (uint8_t)color_fg;
  terminal.color_bg = (uint8_t)color_bg;
  terminal.width = width;
  terminal.height = height;
  terminal.cursor_x = 0;
  terminal.cursor_y = font->height;
  terminal.font = font;
}

void tty_putchar(char ch)
{
  if (ch == '\n')
  {
    terminal.cursor_x = 0;
    terminal.cursor_y += terminal.font->height;
    return;
  }

  vga_drawchar(terminal.font, ch, terminal.cursor_x, terminal.cursor_y, terminal.color_fg, terminal.color_bg);
  terminal.cursor_x += terminal.font->width;

  if (terminal.cursor_x >= terminal.width)
  {
    terminal.cursor_x = 0;
    terminal.cursor_y += terminal.font->height;
    if (terminal.cursor_y >= terminal.height)
    {
      // Implement terminal scroll
      terminal.cursor_y = terminal.font->height;
    }
  }
}

void tty_write(const char *data, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    tty_putchar(data[i]);
  }
}

void tty_writestr(const char *str)
{
  tty_write(str, strlen(str));
}
