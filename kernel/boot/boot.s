##
# Bootsector
##

.extern load_gdt
.global setcs

# multiboot header constants
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

