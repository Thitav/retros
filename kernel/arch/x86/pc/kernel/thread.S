.section .text
.global x86_thread_context_switch
# void x86_thread_context_switch(uint32_t *current_esp, uint32_t new_esp)
x86_thread_context_switch:
    # save callee saved registers
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    # 20(%esp) -> uintptr_t *current_esp
    movl 20(%esp), %eax
    # save current esp in current thread stored esp
    movl %esp, (%eax)
    # 24(%esp) -> uintptr_t new_esp
    movl 24(%esp), %edx
    # switch the esp to the new thread stored esp
    movl %edx, %esp

    # recover callee saved registers
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    ret

.global x86_thread_context_jump
x86_thread_context_jump:
    movl 4(%esp), %edx
    # switch the esp to the new thread stored esp
    movl %edx, %esp

    # recover callee saved registers
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    ret

.global x86_thread_trampoline
x86_thread_trampoline: # (thread_entry_t entry, int argc, char **argv)
    popl %ecx
    
    movw $((4 * 8) | 3), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    movl %esp, %eax
    pushl $((4 * 8) | 3)
    pushl %eax
    pushfl
    pushl $((3 * 8) | 3) # code selector (ring 3 code with bottom 2 bits set for ring 3)
	pushl %ecx # instruction address to return to
	iret
