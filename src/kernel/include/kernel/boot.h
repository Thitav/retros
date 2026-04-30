#ifndef ARCH_BOOT_H
#define ARCH_BOOT_H

#include <stddef.h>
#include <stdint.h>

enum boot_memory_map_status {
    BOOT_MEMORY_MAP_STATUS_AVAILABLE,
    BOOT_MEMORY_MAP_STATUS_RESERVED,
};

struct boot_memory_map {
    uintptr_t base;
    size_t size;
    enum boot_memory_map_status status;
};

struct boot_info {
    size_t memory_maps_length;
    struct boot_memory_map *memory_maps;
    unsigned long int framebuffer_width;
    unsigned long int framebuffer_height;
};

#endif
