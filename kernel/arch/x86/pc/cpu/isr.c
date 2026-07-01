#include "./pic.h"
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>
#include <stdint.h>
#include <x86/cpu/idt.h>
#include <x86/cpu/io.h>
#include <x86/cpu/isr.h>

// defined in isr.S
extern void *isr_stub_table[];

static isr_handler_t isr_handlers[IDT_MAX_ENTRIES] = {NULL};

// should be called before idt_init
void isr_init(struct idt_entry *idt_table) {
    uint16_t i = 0;
    for (i; i < 48; i++) {
        idt_entry(idt_table, i, isr_stub_table[i],
                  IDT_ENTRY_PRESENT | IDT_ENTRY_GATE_INTERRUPT);
    }
    for (i; i < IDT_MAX_ENTRIES; i++) {
        idt_entry(idt_table, i, isr_stub_table[i],
                  IDT_ENTRY_PRESENT | IDT_ENTRY_GATE_INTERRUPT |
                      IDT_ENTRY_DPL_USER);
    }

    pic_remap(32, 40);
}

struct isr_context *isr_entry(struct isr_context *context) {
    pic_eoi();

    if (context->int_no < 32) {
        asm("cli");
        while (1) {
            asm("hlt");
        }
    }

    if (isr_handlers[context->int_no] != NULL) {
        return isr_handlers[context->int_no](context);
    }
    return context;
}

void isr_set_handler(uint8_t int_no, isr_handler_t handler) {
    isr_handlers[int_no] = handler;
}
