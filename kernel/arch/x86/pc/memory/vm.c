#include <kernel/error.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>
#include <stdbool.h>
#include <x86/cpu/paging.h>
#include <x86/memory/vm.h>
#include <x86/vas.h>

static bool tmp_mapped = false;
static uintptr_t lowmap_paddr_base = 0;

#define X86_VM_RECURSIVE_PD_VADDR_BASE                                         \
    ((page_directory_entry_t *)PAGE_INDEXES_VADDR(                             \
        PAGE_RECURSIVE_PDE_INDEX, PAGE_RECURSIVE_PDE_INDEX, 0))
#define X86_VM_RECURSIVE_PT_VADDR_BASE                                         \
    PAGE_INDEXES_VADDR(PAGE_RECURSIVE_PDE_INDEX, 0, 0)

// This returns the virtual address that allows you to
// edit the entries of the page table at pd_index.
static inline page_table_entry_t *x86_vm_pt_vaddr(uint16_t pd_index) {
    return (page_table_entry_t *)(X86_VM_RECURSIVE_PT_VADDR_BASE +
                                  ((uintptr_t)pd_index * PAGE_SIZE));
}

void x86_vm_lowmap_init(uintptr_t paddr, size_t size) {
    if (size > X86_VAS_LOWMAP_SIZE) {
        size = X86_VAS_LOWMAP_SIZE;
    }

    for (size_t i = 0; i < size; i += PAGE_SIZE) {
        x86_vm_map_mem_map(NULL, X86_VAS_LOWMAP_BASE + i, paddr + i);
    }
    lowmap_paddr_base = paddr;
}

void *x86_vm_lowmap_ptr(uintptr_t paddr) {
    ASSERT(paddr >= lowmap_paddr_base, return NULL);
    return (void *)(X86_VAS_LOWMAP_BASE + paddr - lowmap_paddr_base);
}

kerror_t x86_vm_tmp_map(uintptr_t paddr, size_t pages) {
    if (tmp_mapped) {
        return KERROR_VM_TMP_MAP_IN_USE;
    }

    for (size_t i = 0; i < pages; i++) {
        x86_vm_pt_vaddr(X86_VM_TMP_PAGE_PDE_INDEX)[i] =
            (paddr + (i * PAGE_SIZE)) | PAGE_PT_ENTRY_PRESENT |
            PAGE_PT_ENTRY_READWRITE;
    }
    page_directory_reload();
    tmp_mapped = true;
    return KERROR_SUCCESS;
}

kerror_t x86_vm_tmp_unmap(void) {
    if (!tmp_mapped) {
        return KERROR_VM_TMP_MAP_EMPTY;
    }
    tmp_mapped = false;
    return KERROR_SUCCESS;
}

void x86_vm_map_init(struct x86_vm_map *map, uintptr_t pd_paddr) {
    x86_vm_tmp_map(pd_paddr, 1);

    page_table_entry_t *pd = (page_table_entry_t *)(X86_VM_TMP_PAGE_VADDR);

    // copy kernel entries and set other to 0
    memcpy(&pd[PAGE_KERNEL_PD_BASE],
           &X86_VM_RECURSIVE_PD_VADDR_BASE[PAGE_KERNEL_PD_BASE],
           sizeof(page_directory_entry_t) * PAGE_KERNEL_PD_LEN);
    memset(pd, 0, sizeof(page_directory_entry_t) * PAGE_KERNEL_PD_BASE);

    // setup recursive pd
    pd[PAGE_RECURSIVE_PDE_INDEX] =
        pd_paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE;

    x86_vm_tmp_unmap();

    map->page_directory_paddr = pd_paddr;
}

void x86_vm_map_table_map(struct x86_vm_map *map, uintptr_t vaddr,
                          uintptr_t table_paddr) {
    uint16_t pd_idx = page_directory_index(vaddr);

    page_directory_entry_t *pd_entry;
    if (map == NULL) {
        pd_entry = &X86_VM_RECURSIVE_PD_VADDR_BASE[pd_idx];
    } else {
        x86_vm_tmp_map(map->page_directory_paddr, 1);
        pd_entry = &((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];
    }

    // [TODO] handle error
    *pd_entry = table_paddr | PAGE_PD_ENTRY_PRESENT | PAGE_PD_ENTRY_READWRITE |
                PAGE_PD_ENTRY_USER;

    if (map == NULL) {
        page_directory_reload();
    } else {
        x86_vm_tmp_unmap();
    }
}

uintptr_t x86_vm_map_table_unmap(struct x86_vm_map *map, uintptr_t vaddr) {
    uint16_t pd_idx = page_directory_index(vaddr);

    page_directory_entry_t *pd_entry;
    if (map == NULL) {
        pd_entry = &X86_VM_RECURSIVE_PD_VADDR_BASE[pd_idx];
    } else {
        x86_vm_tmp_map(map->page_directory_paddr, 1);
        pd_entry = &((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];
    }

    // [TODO] handle error
    uintptr_t paddr = page_directory_entry_paddr(*pd_entry);
    *pd_entry = 0;

    if (map == NULL) {
        page_directory_reload();
    } else {
        x86_vm_tmp_unmap();
    }

    return paddr;
}

kerror_t x86_vm_map_mem_map(struct x86_vm_map *map, uintptr_t vaddr,
                            uintptr_t paddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_directory_entry_t pd_entry;
    if (map == NULL) {
        pd_entry = X86_VM_RECURSIVE_PD_VADDR_BASE[pd_idx];
    } else {
        x86_vm_tmp_map(map->page_directory_paddr, 1);
        pd_entry = ((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];
        x86_vm_tmp_unmap();
    }

    if ((pd_entry & PAGE_PD_ENTRY_PRESENT) == 0) {
        // [TODO] error
        return KERROR_VM_MISSING_TABLE;
    }

    page_table_entry_t *pt_entry;
    if (map == NULL) {
        pt_entry = &x86_vm_pt_vaddr(pd_idx)[pt_idx];
    } else {
        x86_vm_tmp_map(page_directory_entry_paddr(pd_entry), 1);
        pt_entry = &((page_table_entry_t *)X86_VM_TMP_PAGE_VADDR)[pt_idx];
    }

    *pt_entry = paddr | PAGE_PT_ENTRY_PRESENT | PAGE_PT_ENTRY_READWRITE |
                PAGE_PT_ENTRY_USER;

    if (map == NULL) {
        page_directory_reload();
    } else {
        x86_vm_tmp_unmap();
    }

    return KERROR_SUCCESS;
}

uintptr_t x86_vm_map_mem_unmap(struct x86_vm_map *map, uintptr_t vaddr) {
    uint16_t pd_idx = page_directory_index(vaddr);
    uint16_t pt_idx = page_table_index(vaddr);

    page_table_entry_t *pt_entry;
    if (map == NULL) {
        pt_entry = &x86_vm_pt_vaddr(pd_idx)[pt_idx];
    } else {
        x86_vm_tmp_map(map->page_directory_paddr, 1);
        page_directory_entry_t pd_entry =
            ((page_directory_entry_t *)X86_VM_TMP_PAGE_VADDR)[pd_idx];
        x86_vm_tmp_unmap();

        x86_vm_tmp_map(page_directory_entry_paddr(pd_entry), 1);
        pt_entry = (page_table_entry_t *)X86_VM_TMP_PAGE_VADDR;
    }

    uintptr_t paddr = page_table_entry_paddr(*pt_entry);
    *pt_entry = 0;

    if (map == NULL) {
        page_directory_reload();
    } else {
        x86_vm_tmp_unmap();
    }

    return paddr;
}
