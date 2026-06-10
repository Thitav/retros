#include <kernel/vga.h>

static struct vga_screen screen = {.width = 0, .height = 0};

void vga_init(uint32_t width, uint32_t height)
{
    screen.width = width;
    screen.height = height;
}

void vga_putpixel(uint32_t pos_x, uint32_t pos_y, enum vga_color color)
{
    if (screen.width == 0 || screen.height == 0)
    {
        return;
    }
    VGA_MEMORY[screen.width * pos_y + pos_x] = (uint8_t)color;
}

void vga_fillrect(uint32_t width, uint32_t height, enum vga_color color)
{
    if (screen.width == 0 || screen.height == 0)
    {
        return;
    }
    // Optimize (?) and add rect x and y pos
    uint32_t offset = 0;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            VGA_MEMORY[offset + x] = (uint8_t)color;
        }
        offset += screen.width;
    }
}

void vga_drawchar(const struct bitmap_font *font, char ch,
                  uint32_t pos_x, uint32_t pos_y, enum vga_color color_fg, enum vga_color color_bg)
{
    if (screen.width == 0 || screen.height == 0)
    {
        return;
    }
    uint8_t mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    const uint8_t *glyph = font->bitmap + (font->index[(uint8_t)ch] * font->height);

    for (uint8_t cy = 0; cy < 16; cy++)
    {
        for (uint8_t cx = 0; cx < 8; cx++)
        {
            vga_putpixel(pos_x + cx, pos_y + cy - 12, glyph[cy] & mask[cx] ? color_fg : color_bg);
        }
    }
}
