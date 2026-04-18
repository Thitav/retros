#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#include <stdint.h>
#include <arch/timer.h>

#ifndef __ARCH_INLINE_TIMER_INIT
void timer_init();
#endif
#ifndef __ARCH_INLINE_TIMER_SET
void timer_set(uint16_t delay_ms);
#endif

#endif
