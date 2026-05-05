#ifndef I386_PC_INTERRUPT_H
#define I386_PC_INTERRUPT_H

#include <i386/cpu/isr.h>
#include <stdint.h>

typedef struct isr_context arch_impl_interrupt_context_t;
typedef arch_impl_interrupt_context_t *(*arch_impl_interrupt_handler_t)(arch_impl_interrupt_context_t *context);

static inline void arch_impl_interrupt_set_handler(uint8_t int_no, arch_impl_interrupt_handler_t handler) {
    isr_set_handler(int_no, handler);
}

#endif
