#ifndef X86_VAS_H
#define X86_VAS_H

#include <stdint.h>
#include <x86/cpu/paging.h>
#include <x86/kernel/kstack.h>

#define X86_VAS_KERNEL_BASE (0xC0000000)
#define X86_VAS_KERNEL_SIZE (0x400000)
#define X86_VAS_LIMIT (UINTPTR_MAX - (1024 * PAGE_SIZE * 2))
#define X86_VAS_LOWMAP_BASE (X86_VAS_KERNEL_BASE + X86_VAS_KERNEL_SIZE)
#define X86_VAS_LOWMAP_LIMIT (X86_VAS_LIMIT)
#define X86_VAS_LOWMAP_SIZE (X86_VAS_LOWMAP_LIMIT - X86_VAS_LOWMAP_BASE)

#endif
