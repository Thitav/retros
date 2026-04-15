# void flush_tss(void);
.section .text

.set TSS_SELECTOR, (5 * 8) # index 5 in the gdt

.global tss_flush
tss_flush:
	movw $TSS_SELECTOR, %ax
	ltr %ax
	ret
