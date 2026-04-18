#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>
#include "idt.h"

struct isr_context {
   uint32_t ds;                          // Pushed manually
   uint32_t edi, esi, ebp, esp_ignored, ebx, edx, ecx, eax; // Pushed by pushad
   uint32_t int_no, err_code;                        // Pushed by specific ISR handler
   uint32_t eip, cs, eflags, useresp, ss;            // Pushed by CPU automatically
};

typedef struct isr_context *(*isr_handler_t)(struct isr_context *context); 

void isr_init(struct idt_entry *idt_table);
struct isr_context *isr_entry(struct isr_context *context);
void isr_set_handler(uint8_t int_no, isr_handler_t handler);

#endif
