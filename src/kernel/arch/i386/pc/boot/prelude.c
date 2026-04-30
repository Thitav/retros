#include <stdint.h>
#include "../cpu/paging.h"

#define PRE_PAGING_ADDR(addr) ((uintptr_t)(addr) - KERNEL_BASE)

static page_directory_t kernel_page_directory;
static page_table_t kernel_page_table;

__attribute__((used, section(".boot.text")))
void prelude()
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        *(uintptr_t *)(PRE_PAGING_ADDR(&kernel_page_table[i])) = (i * 0x1000) | PAGE_TABLE_ENTRY_READWRITE | PAGE_TABLE_ENTRY_PRESENT;
    }

    uint32_t directory_entry = PRE_PAGING_ADDR(kernel_page_table) | PAGE_DIRECTORY_ENTRY_READWRITE | PAGE_DIRECTORY_ENTRY_PRESENT;
    *(uintptr_t *)(PRE_PAGING_ADDR(&kernel_page_directory[0])) = directory_entry;
    *(uintptr_t *)(PRE_PAGING_ADDR(&kernel_page_directory[ADDR_TO_PAGE_DIRECTORY_INDEX(KERNEL_BASE)])) = directory_entry;

    page_directory_load((uintptr_t *)PRE_PAGING_ADDR(kernel_page_directory));
    paging_enable();
}
