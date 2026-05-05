.section .text
# void _thread_context_switch_regs(void** old_esp, void* new_esp);
.global _thread_context_switch_regs
_thread_context_switch_regs:
    # 1. Save Callee-Saved Registers (Old Thread)
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    # 2. Save the current ESP into the old TCB's stack pointer field
    # The first argument (old_stack_ptr) is now at 20(%esp)
    # (4 pushes = 16 bytes + 4 bytes for the return address = 20)
    movl 20(%esp), %eax
    movl %esp, (%eax)

    # 3. Load the new thread's ESP from the second argument
    # The second argument (new_stack) is at 24(%esp)
    movl 24(%esp), %esp

    # 4. Restore Callee-Saved Registers (New Thread)
    # We are now on a completely different physical stack!
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    # 5. Return to the caller of 'context_switch' in the new thread
    ret
