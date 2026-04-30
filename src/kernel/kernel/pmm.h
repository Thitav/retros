#ifndef _PMM_H
#define _PMM_H

#include <kernel/boot.h>
#include <stddef.h>

void pmm_init(struct boot_memory_map *memory_maps, size_t memory_maps_length);
uintptr_t pmm_alloc(void);
void pmm_free(uintptr_t addr);

#endif
