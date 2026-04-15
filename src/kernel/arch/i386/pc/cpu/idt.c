#include "idt.h"

// __attribute__((aligned(0x10))) static struct idt_entry idt_entries[IDT_MAX_DESCRIPTORS];
static struct idt_descriptor idtr;

void idt_entry(struct idt_entry* idt_table, uint8_t index, void *isr, uint8_t flags)
{
    struct idt_entry *entry = &idt_table[index];

    entry->isr_low = ((uint32_t)isr) & 0xFFFF;
    entry->kernel_cs = 0x08; // offset for kernel code selector in GDT
    entry->attributes = flags;
    entry->isr_high = ((uint32_t)isr) >> 16;
    entry->reserved = 0;
}

void idt_load(struct idt_entry* idt_table, uint16_t size)
{
    idtr.offset = (uint32_t)idt_table;
    idtr.size = size;
    __asm__ volatile("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile("sti");                   // set the interrupt flag
}
