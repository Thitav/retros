#ifndef ARCH_INTERRUPT_H
#define ARCH_INTERRUPT_H

#include <x86/cpu/isr.h>
#include <stdint.h>

typedef struct isr_context arch_interrupt_context_t;
typedef arch_interrupt_context_t *(*arch_interrupt_handler_t)(arch_interrupt_context_t *context);

static inline void arch_interrupt_set_handler(uint8_t int_no, arch_interrupt_handler_t handler) {
    isr_set_handler(32 + int_no, handler);
}

static inline void arch_interrupt_enable(void) {
    __asm__ __volatile__("sti" : : : "memory");
}

static inline void arch_interrupt_disable(void) {
    __asm__ __volatile__("cli" : : : "memory");
}

#endif
