#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

struct gdt_entry
{
    uint16_t limit_low : 16;
    uint32_t base_low : 24;
    
    uint8_t accessed : 1;             // Accessed bit. The CPU will set it when the segment is accessed unless set to 1 in advance. This means that in case the GDT descriptor is stored in read only pages and this bit is set to 0, the CPU trying to set this bit will trigger a page fault. Best left set to 1 unless otherwise needed.
    uint8_t read_write : 1;           // Readable bit/Writable bit.
                                      //   For code segments: Readable bit. If clear (0), read access for this segment is not allowed. If set (1) read access is allowed. Write access is never allowed for code segments.
                                      //   For data segments: Writeable bit. If clear (0), write access for this segment is not allowed. If set (1) write access is allowed. Read access is always allowed for data segments.
    uint8_t direction_conforming : 1; // Direction bit/Conforming bit. ; For data selectors: Direction bit. If clear (0) the segment grows up. If set (1) the segment grows down, ie. the Offset has to be greater than the Limit.
                                      // For code selectors: Conforming bit.
                                      //   If clear (0) code in this segment can only be executed from the ring set in DPL.
                                      //   If set (1) code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The DPL field represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment where DPL is 2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump from ring 3 to a segment with a DPL of 2 remains in ring 3 after the jump.
    uint8_t executable : 1;           // Executable bit. If clear (0) the descriptor defines a data segment. If set (1) it defines a code segment which can be executed from.
    uint8_t descriptor_type : 1;      // Descriptor type bit. If clear (0) the descriptor defines a system segment (eg. a Task State Segment). If set (1) it defines a code or data segment.
    uint8_t dpl : 2;                  // Descriptor privilege level field. Contains the CPU Privilege level of the segment. 0 = highest privilege (kernel), 3 = lowest privilege (user applications).
    uint8_t present : 1;              // Present bit. Allows an entry to refer to a valid segment. Must be set (1) for any valid segment.
    
    uint8_t limit_high : 4;
    
    uint8_t reserved : 1;    // only used in software; has no effect on hardware
    uint8_t long_mode : 1;   // Long-mode code flag. If set (1), the descriptor defines a 64-bit code segment. When set, DB should always be clear. For any other type of segment (other code types or any data segment), it should be clear (0).
    uint8_t size : 1;        // Size flag. If clear (0), the descriptor defines a 16-bit protected mode segment. If set (1) it defines a 32-bit protected mode segment. A GDT can have both 16-bit and 32-bit selectors at once.
    uint8_t granularity : 1; // Granularity flag, indicates the size the Limit value is scaled by. If clear (0), the Limit is in 1 Byte blocks (byte granularity). If set (1), the Limit is in 4 KiB blocks (page granularity).
    
    uint8_t base_high : 8;
} __attribute__((packed));

extern void gdt_load(uint16_t gdt_limit, uint32_t gdt_start);

#endif
