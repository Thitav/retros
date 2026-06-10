#ifndef KERNEL_MEMORY_FRAME_H
#define KERNEL_MEMORY_FRAME_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct memory_frame {
    uintptr_t paddr;
    size_t size;
    size_t bump;
    bool mapped;
};

void memory_frame_init(struct memory_frame *frame, uintptr_t paddr, size_t size);
uintptr_t memory_frame_alloc(struct memory_frame *frame, size_t size);
struct memory_frame *memory_frame_map(struct memory_frame *frame);

#endif
