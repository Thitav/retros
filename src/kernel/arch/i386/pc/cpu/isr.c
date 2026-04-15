#include "isr.h"
#include "idt.h"
#include "io.h"
#include <stdint.h>
#include <stdio.h>

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

// defined in isr.S
extern void *isr_stub_table[];

void _pic_remap(uint8_t offset1, uint8_t offset2) {
    io_outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4); // starts the initialization sequence (in cascade mode)
    io_wait();
    io_outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();
    io_outb(PIC_MASTER_DATA_PORT, offset1); // ICW2: Master PIC vector offset
    io_wait();
    io_outb(PIC_SLAVE_DATA_PORT, offset2); // ICW2: Slave PIC vector offset
    io_wait();
    io_outb(PIC_MASTER_DATA_PORT, 1 << PIC_CASCADE_IRQ); // ICW3: tell Master PIC that there is a slave PIC at IRQ2
    io_wait();
    io_outb(PIC_SLAVE_DATA_PORT, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    io_outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    io_wait();
    io_outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);
    io_wait();

    // Unmask both PICs.
    io_outb(PIC_MASTER_DATA_PORT, 0);
    io_outb(PIC_SLAVE_DATA_PORT, 0);
}

// should be called before idt_init
void isr_init(struct idt_entry *idt_table)
{
    for (uint8_t i = 0; i < 48; i++)
    {
        idt_entry(idt_table, i, isr_stub_table[i], 0x8E);
    }

    _pic_remap(0x20, 0x28);
}

void isr_handler(uint32_t error, uint32_t interrupt) 
{
    printf("interrupt!\n");
}

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/

