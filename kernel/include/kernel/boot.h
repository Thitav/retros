#ifndef ARCH_BOOT_H
#define ARCH_BOOT_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/memory/vm.h>

struct boot_module {
    uintptr_t paddr;
    size_t size;
};

struct boot_info {
    vm_mapping_t *kernel_mapping;

    size_t modules_len;
    struct boot_module *modules;

    unsigned long int framebuffer_width;
    unsigned long int framebuffer_height;
};

#endif
