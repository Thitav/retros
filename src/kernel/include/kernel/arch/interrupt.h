#ifndef ARCH_INTERRUPT_H
#define ARCH_INTERRUPT_H

#include <stdint.h>
#include <arch/interrupt.h>

#ifndef __ARCH_INLINE_INTERRUPT_SET_HANDLER
void interrupt_set_handler(uint8_t int_no, interrupt_handler_t handler);
#endif

#endif
