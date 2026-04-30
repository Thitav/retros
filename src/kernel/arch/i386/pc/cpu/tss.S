# void flush_tss(void);
.section .text

.set TSS_SELECTOR, (5 * 8) # index 5 in the gdt

.global tss_load
tss_load:
	movw $TSS_SELECTOR, %ax
	ltr %ax
	ret
