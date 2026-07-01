#include <kernel/error.h>
#include <kernel/lib/string.h>
#include <kernel/memory/frame.h>
#include <kernel/memory/vm.h>

void memory_frame_init(struct memory_frame *frame, uintptr_t paddr,
                       size_t size) {
    frame->paddr = paddr;
    frame->size = size;
    frame->bump = 0;
    frame->mapped = false;
}

uintptr_t memory_frame_alloc(struct memory_frame *frame, size_t size) {
    ASSERT(!frame->mapped, return UINTPTR_MAX);
    ASSERT(frame->size - frame->bump >= size, return UINTPTR_MAX);

    uintptr_t paddr = frame->paddr + frame->bump;
    frame->bump += size;
    return paddr;
}

bool memory_frame_map(struct memory_frame *frame) {
    ASSERT(!frame->mapped, return false);
    ASSERT(frame->size == VM_PAGE_SIZE, return false);
    ASSERT(frame->bump == 0, return false);

    frame->mapped = true;
    return true;
}

bool memory_frame_merge(struct memory_frame *frame1,
                        struct memory_frame *frame2) {
    ASSERT(!frame1->mapped, return false);
    ASSERT(!frame2->mapped, return false);
    ASSERT(frame1->paddr + frame1->size == frame2->paddr, return false);
    ASSERT(frame2->bump == 0 || frame1->bump == frame1->size, return false);

    frame1->size += frame2->size;
    frame1->bump += frame2->bump;
    memset(frame2, 0, sizeof(struct memory_frame));
    return true;
}

bool memory_frame_split(struct memory_frame *frame1,
                        struct memory_frame *frame2, size_t size) {
    ASSERT(!frame1->mapped, return false);
    ASSERT(frame2->size == 0, return false);

    ASSERT(size < frame1->size - frame1->bump, return false);
    ASSERT(size % VM_PAGE_SIZE == 0, return false);

    frame2->paddr = frame1->paddr + frame1->size - size;
    frame2->size = size;
    frame2->bump = 0;
    frame2->mapped = false;

    frame1->size -= size;
    return true;
}
