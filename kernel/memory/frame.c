#include <kernel/memory/frame.h>
#include <kernel/memory/vm.h>
#include <kernel/memory/pm.h>

void memory_frame_init(struct memory_frame *frame, uintptr_t paddr, size_t size) {
    frame->paddr = paddr;
    frame->size = size;
    frame->bump = 0;
    frame->mapped = false;
}

uintptr_t memory_frame_alloc(struct memory_frame *frame, size_t size) {
    if (frame->mapped == true) {
        return UINTPTR_MAX;
    }
    
    if (frame->size - frame->bump < size) {
        return UINTPTR_MAX;
    }

    uintptr_t paddr = frame->paddr + frame->bump;
    frame->bump += size;
    return paddr;
}

struct memory_frame *memory_frame_map(struct memory_frame *frame) {
    if (frame->mapped == true) {
        return NULL;
    }

    if (frame->size == VM_PAGE_SIZE && frame->bump == 0) {
        frame->mapped = true;
        return frame;
    }

    if (frame->size - frame->bump - sizeof(struct memory_frame) < VM_PAGE_SIZE) {
        return NULL;
    }

    frame->size -= VM_PAGE_SIZE;

    struct memory_frame *mapped_frame = (struct memory_frame *)pm_kmap_vaddr(memory_frame_alloc(frame, sizeof(struct memory_frame)));
    mapped_frame->paddr = frame->paddr + frame->size;
    mapped_frame->size = VM_PAGE_SIZE;
    mapped_frame->bump = 0;
    mapped_frame->mapped = true;
    return mapped_frame;
}
