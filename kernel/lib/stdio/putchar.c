#include <kernel/lib/stdio.h>

#include <kernel/tty.h>

int putchar(int ic)
{
  char c = (char)ic;
  tty_write(&c, sizeof(c));
  return ic;
}
