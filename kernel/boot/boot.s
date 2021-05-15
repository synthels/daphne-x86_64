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
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; Bootsector
;;

extern load_gdt
global setcs

; multiboot header constants
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO  # flags
.set MAGIC,    0x1BADB002       # magic number
.set CHECKSUM, -(MAGIC + FLAGS) # checksum

# multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# reserve stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# entry point for the kernel
.section .text
.global _start
.type _start, @function
_start:
	jmp load_gdt
	setcs:
		movl $stack_top, %esp
		push %eax
		push %ebx
		call kmain
		cli
		hlt
.size _start, . - _start
