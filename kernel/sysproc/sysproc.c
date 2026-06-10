#include "elf.h"
#include <kernel/sysproc/sysproc.h>
#include <kernel/lib/stdio.h>
#include <kernel/lib/stdlib.h>

void sysproc_load(uintptr_t module_paddr, struct process *sysproc_process, struct thread *sysproc_thread) {
    void *entry = elf32_load(module_paddr);

    char s[16];
    lutoa(entry, s, 16);
    printf(" ENTRY %s", s);

    thread_init(sysproc_thread, sysproc_process, (thread_entry_t)entry, 0, NULL);
}
