#ifndef KERNEL_ARCH_INTERRUPT_H
#define KERNEL_ARCH_INTERRUPT_H

#include <stdint.h>
#include <arch/interrupt.h>

typedef arch_interrupt_handler_t interrupt_handler_t;
typedef arch_interrupt_context_t interrupt_context_t;

static inline void interrupt_set_handler(unsigned char int_no, interrupt_handler_t handler) {
    arch_interrupt_set_handler(int_no, handler);
}

static inline void interrupt_enable(void) {
    arch_interrupt_enable();
}

static inline void interrupt_disable(void) {
    arch_interrupt_disable();
}

#endif
