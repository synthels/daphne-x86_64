;;
 ; Copyright (C) 2022 synthels <synthels.me@gmail.com>
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
 ; SMP trampoline
 ;;

%define TRAMPOLINE_BASE 0x1000
%define SMP_PAGE_TABLE 0x700
%define SMP_STACK 0x750
%define SMP_GDT 0x770
%define SMP_IDT 0x790
%define AP_ENTRY 0x800
%define TRAMPOLINE ap_bootstrap16 + TRAMPOLINE_BASE

[bits 16]
global ap_bootstrap16
ap_bootstrap16:
    cli
    mov ax, 0x0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    o32 lgdt [gdtr32 - TRAMPOLINE]

    ; enable protected mode
    mov eax, cr0
    or al, 0x1
    mov cr0, eax

    jmp 0x8:(ap_bootstrap32 - TRAMPOLINE)

[bits 32]
section .text
ap_bootstrap32:
    mov bx, 0x10
    mov ds, bx
    mov es, bx
    mov ss, bx

    ; load pt
    mov eax, dword [SMP_PAGE_TABLE]
    mov cr3, eax

    ; set pae bit
    mov eax, cr4
    or eax, 1 << 5
    or eax, 1 << 7
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; load idt & gdt64
    lidt [SMP_IDT]
    lgdt [gdtr64 - TRAMPOLINE]
    jmp 8:(ap_bootstrap64 - TRAMPOLINE)

[bits 64]
extern ap_startup
ap_bootstrap64:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, 0x0
    mov fs, ax
    mov gs, ax

    ; load stack & gdt
    mov rsp, [SMP_STACK]
    mov rbp, 0x0
    lgdt [SMP_GDT]

    mov rax, [AP_ENTRY]
    call rax

align 16

; gdt pointers
gdtr32:
  dw gdt32_end - gdt32_start - 1
  dd gdt32_start - TRAMPOLINE
gdtr64:
  dw gdt64_end - gdt64_start - 1
  dq gdt64_start - TRAMPOLINE

; gdt32
gdt32_start:
  dq 0
  dq 0x00CF9A000000FFFF
  dq 0x00CF92000000FFFF
gdt32_end:

; gdt64
gdt64_start:
    dq 0
.code: equ $ - gdt64_start
    dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)
.data: equ $ - gdt64_start
    dq (1<<44) | (1<<47) | (1<<41)
gdt64_end:

global ap_bootstrap_end
ap_bootstrap_end:
