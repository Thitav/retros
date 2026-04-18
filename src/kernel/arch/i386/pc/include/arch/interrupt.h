#ifndef I386_PC_INTERRUPT_H
#define I386_PC_INTERRUPT_H

#include "../cpu/isr.h"
#include <stdint.h>

typedef struct isr_context interrupt_context_t;
typedef interrupt_context_t *(*interrupt_handler_t)(interrupt_context_t *context);

#define __ARCH_INLINE_INTERRUPT_SET_HANDLER
static inline void interrupt_set_handler(uint8_t int_no, interrupt_handler_t handler) {
    isr_set_handler(int_no, handler);
}

#endif
