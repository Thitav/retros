void _do_syscall() {
    __asm__ ("movl $69, %eax");
    __asm__ ("movl $11, %ebx");
    __asm__ ("int $0x80");
}

void _start() {
    _do_syscall();
    while (1) {}
}
