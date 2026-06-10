#include <stdint.h>
#include <x86/cpu/paging.h>

#define PRE_PAGING_ADDR(addr) ((uintptr_t)(addr) - KERNEL_BASE)

page_directory_t kernel_page_directory;
static page_table_t kernel_page_table;
static page_table_t vga_page_table;

__attribute__((used, section(".boot.text")))
void prelude()
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        *(page_table_entry_t *)(PRE_PAGING_ADDR(&kernel_page_table[i])) = (i * 0x1000) | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_PRESENT;
        *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[i])) = 0;
    }

    // [TODO] fix vga mapping
    // *(page_table_entry_t *)(PRE_PAGING_ADDR(&vga_page_table[0])) = 0xA0000 | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_PRESENT;

    page_directory_entry_t directory_entry = PRE_PAGING_ADDR(kernel_page_table) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;
    *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[0])) = directory_entry;
    *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[PAGE_VADDR_PD_INDEX(KERNEL_BASE)])) = directory_entry;
    
    *(page_directory_entry_t *)(PRE_PAGING_ADDR(&kernel_page_directory[1023])) = PRE_PAGING_ADDR(kernel_page_directory) | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_PRESENT;

    page_directory_load((uintptr_t *)PRE_PAGING_ADDR(kernel_page_directory));
    paging_enable();
}
