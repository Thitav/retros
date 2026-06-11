#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define IDT_MAX_ENTRIES 256

#define IDT_ENTRY_GATE_TASK 0b0101 // or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
#define IDT_ENTRY_GATE_INTERRUPT_16 0b0110 // or 0x6: 16-bit Interrupt Gate
#define IDT_ENTRY_GATE_TRAP_16 0b0111 // or 0x7: 16-bit Trap Gate
#define IDT_ENTRY_GATE_INTERRUPT 0b1110 // or 0xE: 32-bit Interrupt Gate
#define IDT_ENTRY_GATE_TRAP 0b1111 // or 0xF: 32-bit Trap Gate
#define IDT_ENTRY_PRESENT (1 << 7)
#define IDT_ENTRY_DPL_USER (0b11 << 5)

struct idt_descriptor {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct idt_entry {
    uint16_t isr_low;   // Lower 16 bits of the ISR address
    uint16_t kernel_cs; // GDT segment selector that will be loaded into CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attributes
    uint16_t isr_high;  // Higher 16 bits of the ISR address
} __attribute__((packed));

void idt_entry(struct idt_entry* idt_table, uint8_t index, void *isr, uint8_t flags);
void idt_load(struct idt_entry* idt_table, uint16_t size);

#endif
