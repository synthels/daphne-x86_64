%define TRAMPOLINE_BASE 0x1000
%define SMP_PAGE_TABLE 0x700
%define SMP_STACK 0x750
%define SMP_GDT 0x770
%define SMP_IDT 0x790

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

    o32 lgdt [pm_gdtr - ap_bootstrap16 + TRAMPOLINE_BASE]

    ; enable protected mode
    mov eax, cr0
    or al, 0x1
    mov cr0, eax

    jmp 0x8:(ap_bootstrap32 - ap_bootstrap16 + TRAMPOLINE_BASE)

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

    ; efet
    mov ecx, 0xc0000080
    rdmsr

    ; enable long mode
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [lm_gdtr - ap_bootstrap16 + TRAMPOLINE_BASE]
    jmp 8:(ap_bootstrap64 - ap_bootstrap16 + TRAMPOLINE_BASE)

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

    lgdt [SMP_GDT]
    lidt [SMP_IDT]

    mov rsp, [SMP_STACK]
    mov rbp, 0x0

    ; reset RFLAGS
    push 0x0
    popf

    call ap_startup

align 16
  pm_gdtr:
    dw pm_gdt_end - pm_gdt_start - 1
    dd pm_gdt_start - ap_bootstrap16 + TRAMPOLINE_BASE

align 16
  pm_gdt_start:
    dq 0                  ; null selector
    dq 0x00CF9A000000FFFF ; cs selector
    dq 0x00CF92000000FFFF ; ds selector
  pm_gdt_end:

align 16
  pm_idtr:
    dw 0
    dd 0
    dd 0
    align 16

align 16
  lm_gdtr:
    dw lm_gdt_end - lm_gdt_start - 1
    dq lm_gdt_start - ap_bootstrap16 + TRAMPOLINE_BASE

align 16
  lm_gdt_start:
    dq 0
    dq 0x00AF98000000FFFF
    dq 0x00CF92000000FFFF
  lm_gdt_end:

global ap_bootstrap_end
ap_bootstrap_end:
