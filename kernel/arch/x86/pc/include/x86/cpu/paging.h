#ifndef _PAGING_H
#define _PAGING_H

#include <kernel/utils/math.h>
#include <stdint.h>
#include <x86/vas.h>

// https://wiki.osdev.org/Paging

#define PAGE_SIZE (0x1000)

#define PAGE_NUMBER_UP(addr) (DIV_UP((addr), PAGE_SIZE))
#define PAGE_NUMBER_DOWN(addr) ((addr) / PAGE_SIZE)

#define PAGE_ALIGN_UP(addr) (ALIGN_UP((addr), PAGE_SIZE))
#define PAGE_ALIGN_DOWN(addr) (ALIGN_DOWN((addr), PAGE_SIZE))

#define PAGE_PD_LEN (1024)
#define PAGE_PT_LEN (1024)

#define PAGE_PD_ENTRY_PRESENT (0b1)
#define PAGE_PD_ENTRY_READWRITE (0b1 << 1)
#define PAGE_PD_ENTRY_USER (0b1 << 2)
#define PAGE_PD_ENTRY_PWT (0b1 << 3)
#define PAGE_PD_ENTRY_PCD (0b1 << 4)
// #define PAGE_PD_ENTRY_ACCESSED (0b1 << 5)
// #define PAGE_PD_ENTRY_PS (0b1 << 7)

#define PAGE_PT_ENTRY_PRESENT (0b1)
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
#define PAGE_INDEXES_VADDR(pd_index, pt_index, offset)                         \
    (PAGE_PD_INDEX_VADDR(pd_index) | PAGE_PT_INDEX_VADDR(pt_index) |           \
     PAGE_VADDR_PT_OFFSET(offset))

#define PAGE_PDE_PADDR(pde)                                                    \
    ((uintptr_t)((page_directory_entry_t)(pde) & 0xFFFFF000))
#define PAGE_PTE_PADDR(pte)                                                    \
    ((uintptr_t)((page_table_entry_t)(pte) & 0xFFFFF000))

#define PAGE_KERNEL_PD_BASE (PAGE_VADDR_PD_INDEX(X86_VAS_KERNEL_BASE))
#define PAGE_KERNEL_PD_LEN (PAGE_PD_LEN - PAGE_KERNEL_PD_BASE)

#define PAGE_RECURSIVE_PDE_INDEX (PAGE_PD_LEN - 1)

// volatile needed since changes to the mapping can change the contents of
// previously created pd/pt entry pointers
typedef uint32_t page_directory_entry_t;
typedef uint32_t page_table_entry_t;

typedef page_directory_entry_t page_directory_t[PAGE_PD_LEN]
    __attribute__((aligned(PAGE_SIZE)));
typedef page_table_entry_t page_table_t[PAGE_PT_LEN]
    __attribute__((aligned(PAGE_SIZE)));

extern page_directory_t kernel_page_directory;
extern page_table_t kernel_page_tables[PAGE_KERNEL_PD_LEN];

static inline uint16_t page_directory_index(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PD_INDEX(vaddr);
}

static inline uintptr_t page_directory_entry_paddr(page_directory_entry_t pde) {
    return PAGE_PDE_PADDR(pde);
}

static inline uint16_t page_table_index(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PT_INDEX(vaddr);
}

static inline uint16_t page_table_offset(uintptr_t vaddr) {
    return (uint16_t)PAGE_VADDR_PT_OFFSET(vaddr);
}

static inline uintptr_t page_table_entry_paddr(page_table_entry_t pte) {
    return PAGE_PTE_PADDR(pte);
}

static inline void page_directory_load(uintptr_t page_directory_paddr) {
    asm volatile("movl %0, %%cr3" : : "r"(page_directory_paddr) : "memory");
}

static inline void page_directory_reload() {
    uint32_t cr3;
    asm volatile("movl %%cr3, %0" : "=r"(cr3));
    asm volatile("movl %0, %%cr3" : : "r"(cr3) : "memory");
}

static inline void paging_enable(void) {
    uint32_t cr0;
    asm volatile("movl %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("movl %0, %%cr0" : : "r"(cr0) : "memory");
}

#endif
