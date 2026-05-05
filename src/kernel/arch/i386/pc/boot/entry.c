#include <i386/cpu/gdt.h>
#include <i386/cpu/idt.h>
#include <i386/cpu/isr.h>
#include <i386/cpu/paging.h>
#include <i386/cpu/tss.h>
#include "./multiboot.h"
#include <kernel/boot.h>
#include <kernel/main.h>
#include <stdint.h>
#include <string.h>

#define BOOT_MEMORY_MAP_MAX_ENTRIES 64

// defined in boot.S
extern uint32_t stack_top;

// one null segment, two ring 0 segments, two ring 3 segments, TSS segment
static struct gdt_entry gdt[6];

static struct boot_info boot_info;
static struct boot_memory_map boot_memory_maps[BOOT_MEMORY_MAP_MAX_ENTRIES];

__attribute__((aligned(0x10))) static struct idt_entry idt_table[IDT_MAX_DESCRIPTORS];

__attribute__((used))
void kernel_entry(uint32_t mb_magic, multiboot_info_t *mb_info) {
    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        return;
    }

    if (!(mb_info->flags >> 6 & 0b1))
    {
        return;
    }

    multiboot_memory_map_t mb_memory_maps[BOOT_MEMORY_MAP_MAX_ENTRIES];
    uint32_t mb_memory_maps_count = mb_info->mmap_length / sizeof(multiboot_memory_map_t);
    if (mb_memory_maps_count > BOOT_MEMORY_MAP_MAX_ENTRIES)
    {
        mb_memory_maps_count = BOOT_MEMORY_MAP_MAX_ENTRIES;
    }
    uint32_t boot_memory_maps_count = mb_memory_maps_count;

    memcpy(mb_memory_maps, (void *)mb_info->mmap_addr, sizeof(multiboot_memory_map_t) * mb_memory_maps_count);

    for (uint32_t i = 1; i < mb_memory_maps_count; i++)
    {
        multiboot_memory_map_t entry = mb_memory_maps[i];

        uint32_t j = i;
        while (j > 0 && mb_memory_maps[j - 1].addr > entry.addr)
        {
            mb_memory_maps[j] = mb_memory_maps[j - 1];
            j--;
        }
        mb_memory_maps[j] = entry;
    }

    boot_memory_maps[0].base = mb_memory_maps[0].addr;
    boot_memory_maps[0].size = mb_memory_maps[0].len;
    if (mb_memory_maps[0].type == MULTIBOOT_MEMORY_AVAILABLE)
    {
        boot_memory_maps[0].status = BOOT_MEMORY_MAP_STATUS_AVAILABLE;
    }
    else
    {
        boot_memory_maps[0].status = BOOT_MEMORY_MAP_STATUS_RESERVED;
    }
    uint32_t current_map_index = 0;
    uint32_t current_map_end = mb_memory_maps[0].addr + mb_memory_maps[0].len;

    /* Loop through the memory map and display the values */
    for (uint32_t i = 1; i < mb_memory_maps_count;
         i++)
    {
        if (mb_memory_maps[i].len == 0)
        {
            boot_memory_maps_count--;
            continue;
        }
        if (mb_memory_maps[i].addr > UINT32_MAX)
        {
            boot_memory_maps_count--;
            continue;
        }
        if (UINT32_MAX - mb_memory_maps[i].addr < mb_memory_maps[i].len)
        {
            mb_memory_maps[i].len = UINT32_MAX - mb_memory_maps[i].addr;
        }

        enum boot_memory_map_status new_map_status;
        if (mb_memory_maps[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            new_map_status = BOOT_MEMORY_MAP_STATUS_AVAILABLE;
        }
        else
        {
            new_map_status = BOOT_MEMORY_MAP_STATUS_RESERVED;
        }

        if (new_map_status == boot_memory_maps[current_map_index].status && mb_memory_maps[i].addr == current_map_end)
        {
            boot_memory_maps[current_map_index].size += mb_memory_maps[i].len;
            current_map_end += mb_memory_maps[i].len;
            continue;
        }

        current_map_index++;
        current_map_end = mb_memory_maps[i].addr + mb_memory_maps[i].len;
        boot_memory_maps[current_map_index].base = mb_memory_maps[i].addr;
        boot_memory_maps[current_map_index].size = mb_memory_maps[i].len;
        boot_memory_maps[current_map_index].status = new_map_status;
    }

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
    tss_load();

    boot_info.memory_maps_length = boot_memory_maps_count;
    boot_info.memory_maps = boot_memory_maps;
    boot_info.framebuffer_width = mb_info->framebuffer_width;
    boot_info.framebuffer_height = mb_info->framebuffer_height;
    kernel_main(&boot_info);
}
