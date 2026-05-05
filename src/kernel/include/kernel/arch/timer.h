#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <stdint.h>
#include <arch/timer.h>

static inline void arch_timer_init(void) {
    arch_impl_timer_init();
}

static inline void arch_timer_set(unsigned delay_ms) {
    arch_impl_timer_set(delay_ms);
}


#endif
