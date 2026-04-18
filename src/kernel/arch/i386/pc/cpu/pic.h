#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>
#include "io.h"

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT (PIC_MASTER_COMMAND_PORT + 1)

#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_SLAVE_DATA_PORT (PIC_SLAVE_COMMAND_PORT + 1)

#define PIC_ICW1_ICW4 0x01      /* Indicates that ICW4 will be present */
#define PIC_ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define PIC_ICW1_INIT 0x10      /* Initialization - required! */

#define PIC_ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define PIC_ICW4_SFNM 0x10       /* Special fully nested (not) */

#define PIC_CASCADE_IRQ 2
#define PIC_EOI 0x20

// sends End of Interrupt (EOI)
static inline void pic_eoi() {
    io_outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void pic_remap(uint8_t offset1, uint8_t offset2);

#endif
