#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

// https://wiki.osdev.org/Paging

#define PAGE_SIZE 0x1000

#define PAGE_PD_ENTRY_PRESENT 0b1
#define PAGE_PD_ENTRY_READWRITE (0b1 << 1)
#define PAGE_PD_ENTRY_USER (0b1 << 2)
#define PAGE_PD_ENTRY_PWT (0b1 << 3)
#define PAGE_PD_ENTRY_PCD (0b1 << 4)
// #define PAGE_PD_ENTRY_ACCESSED (0b1 << 5)
// #define PAGE_PD_ENTRY_PS (0b1 << 7)

#define PAGE_PT_ENTRY_PRESENT 0b1
#define PAGE_PT_ENTRY_READWRITE (0b1 << 1)
#define PAGE_PT_ENTRY_USER (0b1 << 2)
#define PAGE_PT_ENTRY_PWT (0b1 << 3)
#define PAGE_PT_ENTRY_PCD (0b1 << 4)
// #define PAGE_PT_ENTRY_ACCESSED (0b1 << 5)
// #define PAGE_PT_ENTRY_DIRTY (0b1 << 6)
// #define PAGE_PT_ENTRY_PAT (0b1 << 7)
// #define PAGE_PT_ENTRY_GLOBAL (0b1 << 8)

#define PAGE_VADDR_PD_INDEX(addr) ((uintptr_t)(addr) >> 22)
#define PAGE_VADDR_PT_INDEX(addr) (((uintptr_t)(addr) >> 12) & 0x3FF)
#define PAGE_VADDR_PT_OFFSET(addr) ((uintptr_t)(addr) & 0xFFF)

#define PAGE_PD_INDEX_VADDR(index) ((uintptr_t)(index) << 22)
#define PAGE_PT_INDEX_VADDR(index) ((uintptr_t)(index) << 12)
#define PAGE_INDEXES_VADDR(pd_index, pt_index, offset) ((uintptr_t)(pd_index) | (uintptr_t)(pt_index) | ((uintptr_t)(offset) & 0xFFF))

// [TODO] move this somewhere else
#define KERNEL_BASE 0xC0000000

typedef uint32_t page_directory_entry_t;
typedef uint32_t page_table_entry_t;

typedef page_directory_entry_t page_directory_t[1024] __attribute__((aligned(PAGE_SIZE)));
typedef page_table_entry_t page_table_t[1024] __attribute__((aligned(PAGE_SIZE)));

static inline uint16_t page_directory_index(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PD_INDEX(vaddr);
}

static inline uint16_t page_table_index(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PT_INDEX(vaddr);
}

static inline uint16_t page_table_offset(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PT_OFFSET(vaddr);
}

static inline void page_directory_load(page_directory_t pd) {
    __asm__ __volatile__("movl %0, %%cr3" : : "r"(pd) : "memory");
}

static inline void page_directory_reload() {
    uint32_t cr3;
    __asm__ __volatile__("movl %%cr3, %0; movl %0, %%cr3" : "=r"(cr3));
}

static inline void paging_enable(void) {
    uint32_t cr0;
    __asm__ __volatile__("movl %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("movl %0, %%cr0" : : "r"(cr0) : "memory");
}

#endif
