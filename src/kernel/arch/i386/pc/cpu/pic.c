#include "pic.h"
#include "io.h"

void pic_remap(uint8_t offset1, uint8_t offset2) {
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
