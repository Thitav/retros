#include <x86/memory/vm.h>
#include <x86/memory/pm.h>
#include <kernel/lib/string.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>

#define X86_VM_RECURSIVE_PD_INDEX 1023
#define X86_VM_RECURSIVE_PD_VADDR_BASE ((page_directory_entry_t *)PAGE_INDEXES_VADDR(X86_VM_RECURSIVE_PD_INDEX, X86_VM_RECURSIVE_PD_INDEX, 0))
#define X86_VM_RECURSIVE_PT_VADDR_BASE PAGE_INDEXES_VADDR(X86_VM_RECURSIVE_PD_INDEX, 0, 0)

#define X86_VM_TMP_PAGE_PD_INDEX 1022
#define X86_VM_TMP_PAGE_PT_INDEX 1023
#define X86_VM_TMP_PAGE_VADDR PAGE_INDEXES_VADDR(X86_VM_TMP_PAGE_PD_INDEX, X86_VM_TMP_PAGE_PT_INDEX, 0)

#define X86_VM_KERNEL_BASE_PD_INDEX PAGE_VADDR_PD_INDEX(KERNEL_BASE)

// This returns the virtual address that allows you to 
// edit the entries of the page table at pd_index.
static inline page_table_entry_t *x86_vm_pt_vaddr(uint16_t pd_index) {
    return (page_table_entry_t *)(X86_VM_RECURSIVE_PT_VADDR_BASE + ((uintptr_t)pd_index * PAGE_SIZE));
}

static inline void x86_vm_tmp_map(uintptr_t paddr) {
    x86_vm_pt_vaddr(X86_VM_TMP_PAGE_PD_INDEX)[X86_VM_TMP_PAGE_PT_INDEX] = paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE;
    page_directory_reload();
}

static inline void x86_vm_tmp_unmap(void) {
    x86_vm_pt_vaddr(X86_VM_TMP_PAGE_PD_INDEX)[X86_VM_TMP_PAGE_PT_INDEX] = 0;
    page_directory_reload();
}

void arch_vm_mapping_init(struct x86_vm_mapping *mapping) {
    // alloc physical page and tmp map to edit it
    uintptr_t pd_paddr = x86_pm_alloc(1);
    x86_vm_tmp_map(pd_paddr);
    page_table_entry_t *pd = (page_table_entry_t *)(X86_VM_TMP_PAGE_VADDR);

    // copy kernel entries and set other to 0
    memcpy(&pd[X86_VM_KERNEL_BASE_PD_INDEX], &X86_VM_RECURSIVE_PD_VADDR_BASE[X86_VM_KERNEL_BASE_PD_INDEX], sizeof(page_directory_entry_t) * (1024 - X86_VM_KERNEL_BASE_PD_INDEX));
    memset(pd, 0, sizeof(page_directory_entry_t) * X86_VM_KERNEL_BASE_PD_INDEX);

    // setup recursive pd
    pd[X86_VM_RECURSIVE_PD_INDEX] = pd_paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE;

    x86_vm_tmp_unmap();

    mapping->page_directory_paddr = pd_paddr;
}

void arch_vm_mapping_mmap(struct x86_vm_mapping *mapping, uintptr_t vaddr, uintptr_t paddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_directory_entry_t *pd_entry;
    if (mapping != NULL) {
        x86_vm_tmp_map(mapping->page_directory_paddr);
        pd_entry = &((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];
    } else {
        pd_entry = &X86_VM_RECURSIVE_PD_VADDR_BASE[pd_idx];
    }

    bool new_pt = (*pd_entry & PAGE_PD_ENTRY_PRESENT) == 0;
    if (new_pt) {
        uintptr_t new_pt_page = x86_pm_alloc(1);
        // [TODO] handle error
        *pd_entry = new_pt_page | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE | PAGE_PD_ENTRY_USER;

        // Otherwise the CPU will think it's still "Not Present"
        if (mapping == NULL) {
            page_directory_reload(); // Flush the old negative cache
        }
    }

    page_table_entry_t *pt_entry;
    if (mapping != NULL) {
        x86_vm_tmp_map(page_directory_entry_paddr(*pd_entry));
        if (new_pt) {
            memset((void *)X86_VM_TMP_PAGE_VADDR, 0, PAGE_SIZE);
        }
        pt_entry = &((page_table_entry_t *)X86_VM_TMP_PAGE_VADDR)[pt_idx];
    } else {
        if (new_pt) {
            memset(x86_vm_pt_vaddr(pd_idx), 0, PAGE_SIZE);
        }
        pt_entry = &x86_vm_pt_vaddr(pd_idx)[pt_idx];
    }

    *pt_entry = paddr | PAGE_PT_ENTRY_PRESENT | PAGE_PT_ENTRY_READWRITE | PAGE_PT_ENTRY_USER;

    if (mapping != NULL) {
        x86_vm_tmp_unmap();
    } else {
        page_directory_reload();
    }
}

uintptr_t arch_vm_mapping_munmap(struct x86_vm_mapping *mapping, uintptr_t vaddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_table_entry_t *pt_entry;
    if (mapping != NULL) {
        x86_vm_tmp_map(mapping->page_directory_paddr);
        page_directory_entry_t pd_entry = ((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];

        x86_vm_tmp_map(page_directory_entry_paddr(pd_entry));
        pt_entry = (page_table_entry_t *)X86_VM_TMP_PAGE_VADDR;
    } else {
        pt_entry = &x86_vm_pt_vaddr(pd_idx)[pt_idx];
    }

    uintptr_t paddr = page_table_entry_paddr(*pt_entry);
    *pt_entry = 0;

    if (mapping != NULL) {
        x86_vm_tmp_unmap();
    } else {
        page_directory_reload();
    }    
    return paddr;
}
