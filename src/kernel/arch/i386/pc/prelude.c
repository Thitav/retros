#include "cpu/gdt.h"
#include "cpu/paging.h"
#include "cpu/tss.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include <stdint.h>
#include <string.h>

// defined in boot.S
extern uint32_t stack_top;

static page_directory_t identity_page_directory;
static page_table_t identity_page_table;

// one null segment, two ring 0 segments, two ring 3 segments, TSS segment
static struct gdt_entry gdt[6];

__attribute__((aligned(0x10))) static struct idt_entry idt_table[IDT_MAX_DESCRIPTORS];

void prelude(void)
{
    struct gdt_entry *kernel_code = &gdt[1];
    struct gdt_entry *kernel_data = &gdt[2];
    struct gdt_entry *ring3_code = &gdt[3];
    struct gdt_entry *ring3_data = &gdt[4];

    memset(&gdt[0], 0, sizeof(struct gdt_entry));

    kernel_code->limit_low = 0xFFFF;
    kernel_code->base_low = 0;
    kernel_code->accessed = 1;             // better left as 1
    kernel_code->read_write = 1;           // since this is a code segment, specifies that the segment is readable
    kernel_code->direction_conforming = 0; // does not matter for ring 3 as no lower privilege level exists
    kernel_code->executable = 1;           // code segment
    kernel_code->descriptor_type = 1;      // code/data segment
    kernel_code->dpl = 0;                  // ring 3
    kernel_code->present = 1;
    kernel_code->limit_high = 0xF;
    kernel_code->reserved = 1;
    kernel_code->long_mode = 0;
    kernel_code->size = 1;        // it's 32 bits
    kernel_code->granularity = 1; // 4KB page addressing
    kernel_code->base_high = 0;

    *kernel_data = *kernel_code;
    kernel_data->executable = 0;

    ring3_code->limit_low = 0xFFFF;
    ring3_code->base_low = 0;
    ring3_code->accessed = 1;
    ring3_code->read_write = 1;           // since this is a code segment, specifies that the segment is readable
    ring3_code->direction_conforming = 0; // does not matter for ring 3 as no lower privilege level exists
    ring3_code->executable = 1;
    ring3_code->descriptor_type = 1;
    ring3_code->dpl = 3; // ring 3
    ring3_code->present = 1;
    ring3_code->limit_high = 0xF;
    ring3_code->reserved = 1;
    ring3_code->long_mode = 0;
    ring3_code->size = 1;        // it's 32 bits
    ring3_code->granularity = 1; // 4KB page addressing
    ring3_code->base_high = 0;

    *ring3_data = *ring3_code;
    ring3_data->executable = 0;

    tss_write_gdt(&gdt[5]); // TSS segment will be the fifth
    gdt_load(sizeof(gdt), (uint32_t)&gdt[0]);
    
    isr_init(&idt_table[0]);
    idt_load(&idt_table[0], sizeof(idt_table) - 1);

    tss_init(2 * 8, (uint32_t)&stack_top); // 2 * 8 for the kernel data segment (index 2)
    tss_flush();

    for (uint16_t i = 0; i < 1024; i++)
    {
        identity_page_directory[i] = 0b00000010;
        identity_page_table[i] = (i * 0x1000) | 3;
    }
    identity_page_directory[0] = ((uint32_t)identity_page_table) | 3;

    page_directory_load(identity_page_directory);
    paging_enable();
}
