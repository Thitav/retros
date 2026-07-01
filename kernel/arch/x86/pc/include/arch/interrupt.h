#ifndef ARCH_INTERRUPT_H
#define ARCH_INTERRUPT_H

#include <stdint.h>
#include <x86/cpu/isr.h>

typedef struct isr_context arch_interrupt_context_t;
typedef arch_interrupt_context_t *(*arch_interrupt_handler_t)(
    arch_interrupt_context_t *context);

static inline void
arch_interrupt_set_handler(uint8_t int_no, arch_interrupt_handler_t handler) {
    isr_set_handler(32 + int_no, handler);
}

static inline void arch_interrupt_enable(void) {
    asm volatile("sti" : : : "memory");
}

static inline void arch_interrupt_disable(void) {
    asm volatile("cli" : : : "memory");
}

#endif
