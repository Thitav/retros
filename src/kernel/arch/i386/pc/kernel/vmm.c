#include <i386/cpu/paging.h>(
#include <kernel/memory/pmm.h>
#include <string.h>

#define RECURSIVE_PD_INDEX 1023
#define RECURSIVE_PD_VADDR_BASE ((page_directory_entry_t *)PAGE_INDEXES_VADDR(RECURSIVE_PD_INDEX, RECURSIVE_PD_INDEX, 0))
#define RECURSIVE_PT_VADDR_BASE PAGE_INDEXES_VADDR(RECURSIVE_PD_INDEX, 0, 0)

#define TMP_PAGE_PD_INDEX 1022
#define TMP_PAGE_PT_INDEX 1023
#define TMP_PAGE_VADDR PAGE_INDEXES_VADDR(TMP_PAGE_PD_INDEX, TMP_PAGE_PT_INDEX, 0)

#define KERNEL_BASE_PD_INDEX PAGE_VADDR_PD_INDEX(KERNEL_BASE)

// This returns the virtual address that allows you to 
// edit the entries of the page table at pd_index.
static inline page_table_entry_t *_vmm_pt_vaddr(uint16_t pd_index) {
    return (page_table_entry_t *)(RECURSIVE_PT_VADDR_BASE + ((uintptr_t)pd_index * PAGE_SIZE));
}

static inline void _vmm_tmp_map(uintptr_t paddr) {
    _vmm_pt_vaddr(TMP_PAGE_PD_INDEX)[TMP_PAGE_PT_INDEX] = paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE;
    page_directory_reload();
}

static inline void _vmm_tmp_unmap(void) {
    _vmm_pt_vaddr(TMP_PAGE_PD_INDEX)[TMP_PAGE_PT_INDEX] = 0;
    page_directory_reload();
}

uintptr_t arch_impl_vmm_mapping_new(void) {
    // alloc physical page and tmp map to edit it
    uintptr_t pd_paddr = pmm_alloc();
    _vmm_tmp_map(pd_paddr);
    page_table_entry_t *pd = (page_table_entry_t *)(TMP_PAGE_VADDR);

    // copy kernel entries and set other to 0
    memcpy(&pd[KERNEL_BASE_PD_INDEX], &RECURSIVE_PD_VADDR_BASE[KERNEL_BASE_PD_INDEX], sizeof(page_directory_entry_t) * (1024 - KERNEL_BASE_PD_INDEX));
    memset(pd, 0, sizeof(page_directory_entry_t) * KERNEL_BASE_PD_INDEX);

    // setup recursive pd
    pd[RECURSIVE_PD_INDEX] = pd_paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE;

    _vmm_tmp_unmap();

    return pd_paddr;
}

void arch_impl_vmm_map(uintptr_t vaddr, uintptr_t paddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_directory_entry_t *pd_entry = RECURSIVE_PD_VADDR_BASE[pd_idx];
    if (*pd_entry & PAGE_PD_ENTRY_PRESENT == 0) {
        uintptr_t new_pt_page = pmm_alloc();
        *pd_entry = new_pt_page | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_USER;
    }

    _vmm_pt_vaddr(pd_idx)[pt_idx] = paddr | PAGE_PT_ENTRY_PRESENT | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_USER;
    page_directory_reload();
}

uintptr_t arch_impl_vmm_unmap(uintptr_t vaddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_table_entry_t *pt_entry = &_vmm_pt_vaddr(pd_idx)[pt_idx];
    uintptr_t paddr = (uintptr_t)(*pt_entry & 0xfffff);
    *pt_entry = 0;

    page_directory_reload();
    return paddr;
}
