.section .data
gdtr:
    .word 0 # GDT size
    .long 0 # GDT offset

.section .text
.global gdt_load
gdt_load:
    movw 4(%esp), %ax
    movw %ax, gdtr
    movl 8(%esp), %eax
    movl %eax, gdtr+2
    lgdt gdtr
    ljmp $0x08, $gdt_flush

gdt_flush:
    movw $0x10, %ax    # 0x10 is the offset to Kernel Data Segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret
