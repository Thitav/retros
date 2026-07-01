#ifndef KERNEL_VAS_H
#define KERNEL_VAS_H

#include <stddef.h>

void *kheap_malloc(size_t size);
void kheap_free(void *ptr);

#endif
