#include "io.h"
#include <stdint.h>

#define PIT_CHANNEL0_PORT 0x40                    // pic
// #define PIT_CHANNEL1_PORT (PIT_CHANNEL0_PORT + 1) // possibly unimplemented
#define PIT_CHANNEL2_PORT (PIT_CHANNEL0_PORT + 2) // pc speaker
#define PIT_COMMAND_PORT (PIT_CHANNEL0_PORT + 3)

// always use binary mode
// #define PIT_COMMAND_BINARY_MODE 0b0
// #define PIT_COMMAND_BCD_MODE 0b1

#define PIT_COMMAND_OPERATING_MODE_0 (0b000 << 1) // Mode 0 (interrupt on terminal count)
#define PIT_COMMAND_OPERATING_MODE_1 (0b001 << 1) // Mode 1 (hardware re-triggerable one-shot)
#define PIT_COMMAND_OPERATING_MODE_2 (0b010 << 1) // Mode 2 (rate generator)
#define PIT_COMMAND_OPERATING_MODE_3 (0b011 << 1) // Mode 3 (square wave generator)
#define PIT_COMMAND_OPERATING_MODE_4 (0b100 << 1) // Mode 4 (software triggered strobe)
#define PIT_COMMAND_OPERATING_MODE_5 (0b101 << 1) // Mode 5 (hardware triggered strobe)
                                                  // 1 1 0 = Mode 2 (rate generator, same as 0b010)
                                                  // 1 1 1 = Mode 3 (square wave generator, same as 0b011)

#define PIT_COMMAND_ACCESS_MODE_LATCH (0b00 << 4) // Latch count value command
#define PIT_COMMAND_ACCESS_MODE_LO (0b01 << 4)    // Access mode: lobyte only
#define PIT_COMMAND_ACCESS_MODE_HI (0b10 << 4)    // Access mode: hibyte only
#define PIT_COMMAND_ACCESS_MODE_LOHI (0b11 << 4)  // Access mode: lobyte/hibyte

#define PIT_COMMAND_CHANNEL_0 (0b00 << 6)  
// #define PIT_COMMAND_CHANNEL_1 (0b01 << 6) // possibly unimplemented
#define PIT_COMMAND_CHANNEL_2 (0b10 << 6) 
// Read-back command (8254 only)
// #define PIT_COMMAND_READBACK (0b11 << 6)

#define PIT_BINARY_FREQUENCY 1193182
#define PIT_MS_TO_FREQUENCY(ms) ((PIT_BINARY_FREQUENCY * (ms)) / 1000)

static inline void pit_set_command(uint8_t command)
{
    io_outb(PIT_COMMAND_PORT, command);
}

static inline void pit_set_count(uint16_t channel, uint8_t count) {
    io_outb(channel, count);
}

static inline void pit_set_count_lohi(uint16_t channel, uint16_t count) {
    pit_set_count(channel, count & 0xFF);
    pit_set_count(channel, (count & 0xFF00) >> 8);
}
