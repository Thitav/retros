#ifndef KERNEL_CAPABILITY_UNTYPED_FRAME_H
#define KERNEL_CAPABILITY_UNTYPED_FRAME_H

#include <stdint.h>
#include <stddef.h>

struct untyped_frame {
    uintptr_t base_paddr;
    size_t size;
    size_t bump;
};

void untyped_frame_init(struct untyped_frame *frame, uintptr_t base_paddr, size_t size);
void *untyped_frame_retype(struct untyped_frame *frame, size_t size);

#endif
