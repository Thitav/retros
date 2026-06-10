#include "./multiboot.h"
#include <kernel/boot.h>
#include <kernel/lib/string.h>
#include <kernel/main.h>
#include <stdint.h>
#include <x86/cpu/gdt.h>
#include <x86/cpu/idt.h>
#include <x86/cpu/isr.h>
#include <x86/cpu/paging.h>
#include <x86/cpu/tss.h>
#include <x86/memory/pm.h>

// defined in boot.S
extern uint32_t stack_top;

// one null segment, two ring 0 segments, two ring 3 segments, TSS segment
static struct gdt_entry gdt[6];

static struct x86_vm_mapping kernel_mapping;
#define MAX_BOOT_MODULES (1)
static struct boot_module boot_modules[MAX_BOOT_MODULES];
static struct boot_info boot_info;

__attribute__((aligned(0x10))) static struct idt_entry idt_table[IDT_MAX_DESCRIPTORS];

#define VM_MAP_BASE (PAGE_VADDR_PD_INDEX(KERNEL_BASE) + 1)
#define VM_MAP_MAX (PAGE_VADDR_PD_INDEX(UINTPTR_MAX) - 1)

__attribute__((used)) void kernel_entry(uint32_t mb_magic, multiboot_info_t *mb_info)
{
    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        return;
    }

    if (!(mb_info->flags & (1 << 6)))
    {
        return;
    }

    if (!(mb_info->flags & (1 << 3)))
    {
        return;
    }

    // initialize the available physical memory areas and do the lower mapping
    size_t vm_map = VM_MAP_BASE;
    for (size_t offset = 0; offset < mb_info->mmap_length; offset += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *mb_memory_map = (multiboot_memory_map_t *)(mb_info->mmap_addr + offset);
        if (mb_memory_map->type != MULTIBOOT_MEMORY_AVAILABLE)
        {
            continue;
        }

        size_t base_page;
        if (mb_memory_map->addr < 0x400000)
        {
            base_page = 0x400000 / PAGE_SIZE;
        }
        else
        {
            base_page = (mb_memory_map->addr + PAGE_SIZE - 1) / PAGE_SIZE;
        }

        for (size_t page = base_page; page < mb_memory_map->len / PAGE_SIZE; page++)
        {
            if (vm_map <= VM_MAP_MAX)
            {
                kernel_page_directory[vm_map] = (page * PAGE_SIZE) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;
                vm_map++;
            }

            x86_pm_set_available(page);
        }
    }

    uint32_t modules_len;
    if (mb_info->mods_count > MAX_BOOT_MODULES)
    {
        modules_len = MAX_BOOT_MODULES;
    }
    else
    {
        modules_len = mb_info->mods_count;
    }

    multiboot_module_t *mb_modules = (multiboot_module_t *)mb_info->mods_addr;
    for (uint32_t i = 0; i < modules_len; i++)
    {
        for (size_t page = mb_modules[i].mod_start / PAGE_SIZE; page < (mb_modules[i].mod_end + PAGE_SIZE - 1) / PAGE_SIZE; page++) {
            x86_pm_set_reserved(page);
        }
        boot_modules[i] = (struct boot_module){.paddr = mb_modules[i].mod_start, .size = mb_modules[i].mod_end - mb_modules[i].mod_start};
    }
    // [TODO] clean this up
    x86_pm_set_reserved(0xA0000 / PAGE_SIZE);
    boot_info.modules = &boot_modules;
    boot_info.modules_len = modules_len;

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

    boot_info.framebuffer_width = mb_info->framebuffer_width;
    boot_info.framebuffer_height = mb_info->framebuffer_height;

    kernel_mapping.page_directory_paddr = (uintptr_t)kernel_page_directory - KERNEL_BASE;
    boot_info.kernel_mapping = &kernel_mapping;

    __asm__("addl $0xC0000000, %ebp");
    __asm__("addl $0xC0000000, %esp");

    // identity unmap
    kernel_page_directory[0] = 0;
    page_directory_reload();

    kernel_main(&boot_info);
}
