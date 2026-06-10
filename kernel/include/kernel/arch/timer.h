#ifndef KERNEL_ARCH_TIMER_H
#define KERNEL_ARCH_TIMER_H

#include <stdint.h>
#include <arch/timer.h>

static inline void timer_init(void) {
    arch_timer_init();
}

static inline void timer_set(unsigned delay_ms) {
    arch_timer_set(delay_ms);
}

#endif
