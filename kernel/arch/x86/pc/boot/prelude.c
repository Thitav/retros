#include <stdint.h>
#include <x86/cpu/paging.h>
#include <x86/vas.h>

#define PRE_PAGING_ADDR(addr) ((uintptr_t)(addr) - X86_VAS_KERNEL_BASE)

page_directory_t kernel_page_directory = {0};
page_table_t kernel_page_tables[PAGE_KERNEL_PD_LEN] = {0};

// static page_table_t vga_page_table;

__attribute__((used, section(".boot.text"))) void prelude()
{
    for (uint16_t i = 0; i < PAGE_PT_LEN; i++)
    {
        *(page_table_entry_t *)(PRE_PAGING_ADDR(&kernel_page_tables[0][i])) = (i * PAGE_SIZE) | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_PRESENT;
    }

    for (uint16_t i = 0; i < PAGE_KERNEL_PD_LEN; i++)
    {
        *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[PAGE_KERNEL_PD_BASE + i])) = PRE_PAGING_ADDR(&kernel_page_tables[i]) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;
    }

    // [TODO] fix vga mapping
    // *(page_table_entry_t *)(PRE_PAGING_ADDR(&vga_page_table[0])) = 0xA0000 | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_PRESENT;

    *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[0])) = PRE_PAGING_ADDR(&kernel_page_tables[0]) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;
    *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[PAGE_RECURSIVE_PDE_INDEX])) = PRE_PAGING_ADDR(kernel_page_directory) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;

    page_directory_load(PRE_PAGING_ADDR(kernel_page_directory));
    paging_enable();
}
