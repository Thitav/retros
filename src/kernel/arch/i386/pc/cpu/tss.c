#include "tss.h"
#include <string.h>

static struct tss_entry tss_entry;

// https://wiki.osdev.org/Getting_to_Ring_3

void tss_write_gdt(struct gdt_entry *gdt)
{
    // Compute the base and limit of the TSS for use in the GDT entry.
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = sizeof(tss_entry);

    // Add a TSS descriptor to the GDT.
    gdt->limit_low = limit;
    gdt->base_low = base;
    gdt->accessed = 1;               // With a system entry (`code_data_segment` = 0), 1 indicates TSS and 0 indicates LDT
    gdt->read_write = 0;             // For a TSS, indicates busy (1) or not busy (0).
    gdt->direction_conforming = 0; // always 0 for TSS
    gdt->executable = 1;                   // For a TSS, 1 indicates 32-bit (1) or 16-bit (0).
    gdt->descriptor_type = 0;      // indicates TSS/LDT (see also `accessed`)
    gdt->dpl = 0;                    // ring 0, see the comments below
    gdt->present = 1;
    gdt->limit_high = (limit & (0xf << 16)) >> 16; // isolate top nibble
    gdt->reserved = 0;                            // 0 for a TSS
    gdt->long_mode = 0;
    gdt->size = 0;                                 // should leave zero according to manuals.
    gdt->granularity = 0;                                // limit is in bytes, not pages
    gdt->base_high = (base & (0xff << 24)) >> 24; // isolate top byte
}

void tss_init(uint32_t kernel_ss, uint32_t kernel_sp) {
        // Ensure the TSS is initially zero'd.
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = kernel_ss;   // Set the kernel stack segment.
    tss_entry.esp0 = kernel_sp; // Set the kernel stack pointer.
                                                   // note that CS is loaded from the IDT entry and should be the regular kernel code segment
}
