global enter_usermode

enter_usermode:
	push 0x0
	mov rax, qword 0x7f7ffffffff0
	push rax
	pushfq
	push 0x1b
	mov rax, qword umode
	push rax
	iretq
umode:
	ret