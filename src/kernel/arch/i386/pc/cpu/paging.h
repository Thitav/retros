#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

// https://wiki.osdev.org/Paging

#define PAGE_DIRECTORY_ENTRY_PRESENT 0b1
#define PAGE_DIRECTORY_ENTRY_READWRITE (0b1 << 1)
#define PAGE_DIRECTORY_ENTRY_USER (0b1 << 2)
#define PAGE_DIRECTORY_ENTRY_PWT (0b1 << 3)
#define PAGE_DIRECTORY_ENTRY_PCD (0b1 << 4)
// #define PAGE_DIRECTORY_ENTRY_ACCESSED (0b1 << 5)
// #define PAGE_DIRECTORY_ENTRY_PS (0b1 << 7)

#define PAGE_TABLE_ENTRY_PRESENT 0b1
#define PAGE_TABLE_ENTRY_READWRITE (0b1 << 1)
#define PAGE_TABLE_ENTRY_USER (0b1 << 2)
#define PAGE_TABLE_ENTRY_PWT (0b1 << 3)
#define PAGE_TABLE_ENTRY_PCD (0b1 << 4)
// #define PAGE_TABLE_ENTRY_ACCESSED (0b1 << 5)
// #define PAGE_TABLE_ENTRY_DIRTY (0b1 << 6)
// #define PAGE_TABLE_ENTRY_PAT (0b1 << 7)
// #define PAGE_TABLE_ENTRY_GLOBAL (0b1 << 8)

#define ADDR_TO_PAGE_DIRECTORY_INDEX(addr) ((uintptr_t)(addr) >> 22)
#define ADDR_TO_PAGE_TABLE_INDEX(addr) (((uintptr_t)(addr) >> 12) & 0x3FF)
#define ADDR_PAGE_OFFSET(addr) ((uintptr_t)(addr) & 0xFFF)

#define KERNEL_BASE 0xC0000000

typedef uint32_t page_directory_t[1024] __attribute__((aligned(0x1000)));
typedef uint32_t page_table_t[1024] __attribute__((aligned(0x1000)));

static inline void page_directory_load(page_directory_t page_directory) {
    __asm__ __volatile__("movl %0, %%cr3" : : "r"(page_directory) : "memory");
}

static inline void paging_enable(void) {
    uint32_t cr0;
    __asm__ __volatile__("movl %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("movl %0, %%cr0" : : "r"(cr0) : "memory");
}

#endif
