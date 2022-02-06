/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * x64 CPU functions
 */

#include "cpu.h"

void arch_swapregs(regs_t *regs)
{
    asm volatile (
        "mov %%rax, %0\n"
        "mov %%rbx, %1\n"
        "mov %%rcx, %2\n"
        "mov %%rdx, %3\n"
        "mov %%rsi, %4\n"
        "mov %%rdi, %5\n"
        "mov %%rbp, %6\n"
        "mov %%r8,  %7\n"
        "mov %%r9,  %8\n"
        "mov %%r10, %9\n"
        "mov %%r11, %10\n"
        "mov %%r12, %11\n"
        "mov %%r13, %12\n"
        "mov %%r14, %13\n"
        "mov %%r15, %14\n"
        : : "rm"(regs->rax),
            "rm"(regs->rbx),
            "rm"(regs->rcx),
            "rm"(regs->rdx),
            "rm"(regs->rsi),
            "rm"(regs->rdi),
            "rm"(regs->rbp),
            "rm"(regs->r8),
            "rm"(regs->r9),
            "rm"(regs->r10),
            "rm"(regs->r11),
            "rm"(regs->r12),
            "rm"(regs->r13),
            "rm"(regs->r14),
            "rm"(regs->r15)
    );
}

bool cpu_has_apic(void)
{
    uint32_t u, eax, edx;
    __get_cpuid(1, &eax, &u, &u, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}
