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

align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; reserve stack
STACKSIZE equ 16384

section .bss
align 16
stack_bottom:
resb STACKSIZE
stack_top:

section .text
; kernel entry point
_start:
	mov esp, stack_top
	push eax
	push ebx
	call load_gdt
	call kmain
	halt:
		cli
		hlt
