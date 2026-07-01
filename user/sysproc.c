void _do_syscall(int arg) {
    __asm__ __volatile__ (
        "int $0x80"
        : /* No outputs */
        : "a" (69),    /* Put 69 into %eax */
          "b" (arg)    /* Put 'arg' into %ebx */
        : "memory"     /* Tell the compiler memory might change */
    );
}

void _start(int arg_size, void *arg) {
    _do_syscall(arg_size);
    _do_syscall(*(int*)arg);
    while (1) {}
}
