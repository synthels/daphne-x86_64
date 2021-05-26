extern load_gdt
extern kmain

global _start
global setcs

section .multiboot
MODULEALIGN	equ  1<<0
MEMINFO		equ  1<<1
FLAGS		equ  MODULEALIGN | MEMINFO
MAGIC		equ  0x1badb002
CHECKSUM	equ -(MAGIC + FLAGS)

section .text
; reserve stack
STACKSIZE equ 0x4000
; kernel entry point
_start:
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	jmp load_gdt
	setcs:
		mov esp, stack + STACKSIZE
		push eax
		push ebx
		call kmain
	halt:
		cli
		hlt

section .bss
align 4
stack: resb STACKSIZE
