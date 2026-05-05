#ifndef KERNEL_MEMORY_PMM_H
#define KERNEL_MEMORY_PMM_H

#include <kernel/boot.h>
#include <stddef.h>

void pmm_init(struct boot_memory_map *memory_maps, size_t memory_maps_length);
size_t pmm_available_pages(void);
uintptr_t pmm_alloc(void);
void pmm_free(uintptr_t addr);

#endif
