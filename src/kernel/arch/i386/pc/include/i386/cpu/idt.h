#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

struct idt_descriptor
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct idt_entry
{
    uint16_t isr_low;   // Lower 16 bits of the ISR address
    uint16_t kernel_cs; // GDT segment selector that will be loaded into CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attributes
    uint16_t isr_high;  // Higher 16 bits of the ISR address
} __attribute__((packed));

void idt_entry(struct idt_entry* idt_table, uint8_t index, void *isr, uint8_t flags);
void idt_load(struct idt_entry* idt_table, uint16_t size);

#endif
