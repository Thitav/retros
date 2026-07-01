.section .text

.set thread_context_esp, 0
.set thread_context_kernel_esp, 4

.global x86_thread_context_switch
# void x86_thread_context_switch(x86_thread_context *current_context, x86_thread_context *next_context)
x86_thread_context_switch:
    # save callee saved registers
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    # 20(%esp) -> struct x86_thread_context *current_context
    movl 20(%esp), %ebx
    # save current esp in current thread stored esp
    movl %esp, thread_context_esp(%ebx)
    # 24(%esp) -> struct x86_thread_context *next_context
    movl 24(%esp), %ebx
    # switch the esp to the new thread stored esp
    pushl thread_context_kernel_esp(%ebx)
    call tss_set_sp

    movl thread_context_esp(%ebx), %esp

    # recover callee saved registers
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    ret

.global x86_thread_context_jump
x86_thread_context_jump:
    movl 4(%esp), %ebx
    # switch the esp to the new thread stored esp

    pushl thread_context_kernel_esp(%ebx)
    call tss_set_sp

    movl thread_context_esp(%ebx), %esp

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
