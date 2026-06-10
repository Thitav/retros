#include <kernel/capability/untyped_frame.h>
// #include <kernel/memory/pmm.h>

void untyped_frame_init(struct untyped_frame *frame, uintptr_t base_paddr, size_t size)
{
    *frame = (struct untyped_frame){.base_paddr = base_paddr, .size = size, .bump = 0};
}

void *untyped_frame_retype(struct untyped_frame *frame, size_t size)
{   
    if (frame->size - frame->bump < size) {
        return NULL;
    }

    void *ptr = (void *)frame->bump;
    frame->bump += size;
    return ptr;
}
