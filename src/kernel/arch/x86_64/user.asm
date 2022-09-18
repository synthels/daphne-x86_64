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
 ; Switch to CPL0
 ;;

[bits 64]
global arch_features_enable
arch_features_enable:
    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 0
    wrmsr
    ret

global arch_reload_page_table
arch_reload_page_table:
    mov cr3, rdi
    ret

global arch_switch_to_user
arch_switch_to_user:
    mov rbp, [rsi + 72]
    mov rax, [rsi + 120]
    mov rdx, [rsi + 96]
    mov rbx, [rsi + 112]
    mov r12, [rsi + 32]
    mov r13, [rsi + 24]
    mov r14, [rsi + 16]
    mov r15, [rsi + 8]
    mov rsp, [rsi + 160]

    mov rcx, rdi
    mov r11, 0x202

    o64 sysret
