#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>
#include "idt.h"

void isr_init(struct idt_entry *idt_table);
void isr_handler(uint32_t error, uint32_t interrupt);

#endif
