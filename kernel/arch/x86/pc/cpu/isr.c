#include <x86/cpu/isr.h>
#include <x86/cpu/idt.h>
#include <x86/cpu/io.h>
#include "./pic.h"
#include <stdint.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>

// defined in isr.S
extern void *isr_stub_table[];

static isr_handler_t isr_handlers[IDT_MAX_DESCRIPTORS] = { NULL };

// should be called before idt_init
void isr_init(struct idt_entry *idt_table)
{
    for (uint16_t i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        idt_entry(idt_table, i, isr_stub_table[i], 0x8E);
    }

    pic_remap(0x20, 0x28);
}

struct isr_context *isr_entry(struct isr_context *context) 
{
    pic_eoi();

    if (context->int_no < 32) {
        __asm__ ("cli");
        while (1) {
            __asm__ ("hlt");
        }
    }
    // char s[16];
    // lutoa(context->int_no, s, 10);
    // printf(" %s ", s);

    if (isr_handlers[context->int_no] != NULL) {
        return isr_handlers[context->int_no](context);
    }
    return context;
}

void isr_set_handler(uint8_t int_no, isr_handler_t handler) {
    isr_handlers[int_no] = handler;
}
