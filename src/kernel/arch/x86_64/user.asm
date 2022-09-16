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
global cpu_switch_to_user
cpu_switch_to_user:
    mov rsp, [rsi + 136]
    mov rbp, [rsi + 8],
    mov eax, [rsi + 24]
    mov edx, [rsi + 28]
    mov rcx, 0xc0000100
    wrmsr
    mov rbx, [rsi + 32]
    mov r12, [rsi + 40]
    mov r13, [rsi + 48]
    mov r14, [rsi + 56]
    mov r15, [rsi + 64]

    mov rcx, 0xc0000082
    wrmsr
    mov rcx, 0xc0000080
    rdmsr
    or eax, 1
    wrmsr
    mov rcx, 0xc0000081
    rdmsr
    mov edx, 0x00180008
    wrmsr

    mov rcx, rdi
    mov r11, 0x202
    o64 sysret
