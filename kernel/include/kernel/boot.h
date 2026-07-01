#ifndef ARCH_BOOT_H
#define ARCH_BOOT_H

#include <kernel/memory/vm.h>
#include <stddef.h>
#include <stdint.h>

struct boot_module {
    uintptr_t paddr;
    size_t size;
};

struct boot_pm_map {
    uintptr_t paddr;
    size_t size;
};

struct boot_info {
    size_t modules_len;
    struct boot_module *modules;

    size_t pm_map_len;
    struct boot_pm_map *pm_map;
    size_t lowmap_index;

    unsigned long int framebuffer_width;
    unsigned long int framebuffer_height;
};

#endif
