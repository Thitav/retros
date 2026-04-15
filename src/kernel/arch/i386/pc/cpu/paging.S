.section .text
.global page_directory_load
page_directory_load:
  pushl %ebp
  movl %esp, %ebp
  movl 8(%esp), %eax
  movl %eax, %cr3
  movl %ebp, %esp
  popl %ebp
  ret

.global paging_enable
paging_enable:
  pushl %ebp
  movl %esp, %ebp
  movl %cr0, %eax
  orl $0x80000000, %eax
  movl %eax, %cr0  
  movl %ebp, %esp
  popl %ebp
  ret
