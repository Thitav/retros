#include <kernel/arch/interrupt.h>
#include <kernel/lib/stdio.h>

void panic(char *message) {
    arch_interrupt_disable();
    printf("[PANIC] %s\n", message);
    while (1) {
        // [TODO] remove arch specific instruction
        asm("hlt");
    }
}
