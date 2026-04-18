.section .text

.extern isr_entry

isr_common_stub:
    # save state
    pushal

    movw %ds, %ax
    pushw %ax
    
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    
    # SysV ABI requirement
    cld

    # call handler
    pushl %esp
    call isr_entry
    # function return new esp
    movl %eax, %esp

    # restore state
    popw %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    
    popal
    
    # handle pushed error and interrupt code
    addl $8, %esp
    iret

.macro ISR_ERROR_STUB n
.align 4
isr_stub_\n:
    # cpu already pushed error code
    push $\n # using push without a size suffix so that stack aligns correctly
    jmp isr_common_stub
.endm

.macro ISR_NO_ERROR_STUB n
.align 4
isr_stub_\n:
    push $0 # push dummy error code to mantain stack consistent with error cases
    push $\n
    jmp isr_common_stub
.endm

ISR_NO_ERROR_STUB 0 # divide by zero exception
ISR_NO_ERROR_STUB 1 # debug exception
ISR_NO_ERROR_STUB 2 # non maskable interrupt exception
ISR_NO_ERROR_STUB 3 # int 3 exception
ISR_NO_ERROR_STUB 4 # INTO exception
ISR_NO_ERROR_STUB 5 # out of bounds exception
ISR_NO_ERROR_STUB 6 # invalid opcode exception
ISR_NO_ERROR_STUB 7 # coprocessor not available exception
ISR_ERROR_STUB    8 # double fault exception
ISR_NO_ERROR_STUB 9 # coprocessor segment overrun exception
ISR_ERROR_STUB    10 # bad tss exception
ISR_ERROR_STUB    11 # segment not present exception
ISR_ERROR_STUB    12 # stack fault exception
ISR_ERROR_STUB    13 # general protection fault exception
ISR_ERROR_STUB    14 # page fault exception
ISR_NO_ERROR_STUB 15 # reserved exception
ISR_NO_ERROR_STUB 16 # floating point exception
ISR_ERROR_STUB    17 # alignment check exception
ISR_NO_ERROR_STUB 18 # machine check exception
ISR_NO_ERROR_STUB 19 # reserved
ISR_NO_ERROR_STUB 20 # reserved
ISR_NO_ERROR_STUB 21 # reserved
ISR_NO_ERROR_STUB 22 # reserved
ISR_NO_ERROR_STUB 23 # reserved
ISR_NO_ERROR_STUB 24 # reserved
ISR_NO_ERROR_STUB 25 # reserved
ISR_NO_ERROR_STUB 26 # reserved
ISR_NO_ERROR_STUB 27 # reserved
ISR_NO_ERROR_STUB 28 # reserved
ISR_NO_ERROR_STUB 29 # reserved
ISR_ERROR_STUB    30 # reserved
ISR_NO_ERROR_STUB 31 # reserved

# hardware interrupts
ISR_NO_ERROR_STUB 32
ISR_NO_ERROR_STUB 33
ISR_NO_ERROR_STUB 34
ISR_NO_ERROR_STUB 35
ISR_NO_ERROR_STUB 36
ISR_NO_ERROR_STUB 37
ISR_NO_ERROR_STUB 38
ISR_NO_ERROR_STUB 39
ISR_NO_ERROR_STUB 40
ISR_NO_ERROR_STUB 41
ISR_NO_ERROR_STUB 42
ISR_NO_ERROR_STUB 43
ISR_NO_ERROR_STUB 44
ISR_NO_ERROR_STUB 45
ISR_NO_ERROR_STUB 46
ISR_NO_ERROR_STUB 47

.section .data
.align 4
.global isr_stub_table
isr_stub_table:
  .altmacro
  .macro ISR_STUB i
  .long isr_stub_\i
  .endm
  
  .set i, 0
  .rept 48
    ISR_STUB %i
    .set i, i + 1
  .endr
