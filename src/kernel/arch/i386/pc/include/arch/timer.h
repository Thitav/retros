#ifndef I386_PC_TIMER_H
#define I386_PC_TIMER_H

#include "../cpu/pit.h"
#include <stdint.h>

#define __ARCH_INLINE_TIMER_INIT
static inline void timer_init() {
    pit_set_command(PIT_COMMAND_CHANNEL_0 | PIT_COMMAND_ACCESS_MODE_LOHI | PIT_COMMAND_OPERATING_MODE_3);
}

#define __ARCH_INLINE_TIMER_SET
static inline void timer_set(uint16_t delay_ms) {
    pit_set_count_lohi(PIT_CHANNEL0_PORT, PIT_MS_TO_FREQUENCY(delay_ms));
}

#endif
