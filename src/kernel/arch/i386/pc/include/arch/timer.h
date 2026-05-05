#ifndef I386_PC_TIMER_H
#define I386_PC_TIMER_H

#include <i386/cpu/pit.h>
#include <stdint.h>

static inline void arch_impl_timer_init(void) {
    pit_set_command(PIT_COMMAND_CHANNEL_0 | PIT_COMMAND_ACCESS_MODE_LOHI | PIT_COMMAND_OPERATING_MODE_3);
}

static inline void arch_impl_timer_set(uint16_t delay_ms) {
    pit_set_count_lohi(PIT_CHANNEL0_PORT, PIT_MS_TO_FREQUENCY(delay_ms));
}

#endif
