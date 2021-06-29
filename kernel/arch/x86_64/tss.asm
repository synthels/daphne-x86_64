[bits 64]

global flush_tss

flush_tss:
	mov ax, (5 * 8) | 3
	ltr ax
	ret