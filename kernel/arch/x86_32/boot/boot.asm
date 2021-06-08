;;
; Copyright (C) 2020 synthels <synthels.me@gmail.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; boot
;;

[bits 32]

extern kmain

global _start
global setcs
global stack_top

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
	call kmain
	halt:
		cli
		hlt
