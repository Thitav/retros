#ifndef ARCH_INTERRUPT_H
#define ARCH_INTERRUPT_H

#include <stdint.h>
#include <arch/interrupt.h>

typedef arch_impl_interrupt_handler_t arch_interrupt_handler_t;
typedef arch_impl_interrupt_context_t arch_interrupt_context_t;

static inline void arch_interrupt_set_handler(unsigned char int_no, arch_interrupt_handler_t handler) {
    arch_impl_interrupt_set_handler(int_no, handler);
}


#endif
