#include "isr.h"
#include "idt.h"
#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include "pic.h"
#include <string.h>

// defined in isr.S
extern void *isr_stub_table[];

isr_handler_t isr_handlers[IDT_MAX_DESCRIPTORS];

// should be called before idt_init
void isr_init(struct idt_entry *idt_table)
{
    for (uint8_t i = 0; i < 48; i++)
    {
        isr_handlers[i] = NULL;
        idt_entry(idt_table, i, isr_stub_table[i], 0x8E);
    }

    pic_remap(0x20, 0x28);
}

struct isr_context *isr_entry(struct isr_context *context) 
{
    pic_eoi();

    if (isr_handlers[context->int_no] != NULL) {
        return isr_handlers[context->int_no](context);
    }
    return context;
}

void isr_set_handler(uint8_t int_no, isr_handler_t handler) {
    isr_handlers[int_no] = handler;
}
