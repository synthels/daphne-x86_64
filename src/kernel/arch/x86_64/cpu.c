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

void save_registers(regs_t *buf, regs_t *r)
{
    buf->r15 = r->r15;
    buf->r14 = r->r14;
    buf->r13 = r->r13;
    buf->r12 = r->r12;
    buf->r11 = r->r11;
    buf->r10 = r->r10;
    buf->r9 = r->r9;
    buf->r8 = r->r8;
    buf->rbp = r->rbp;
    buf->rdi = r->rdi;
    buf->rsi = r->rsi;
    buf->rdx = r->rdx;
    buf->rcx = r->rcx;
    buf->rbx = r->rbx;
    buf->rax = r->rax;
    buf->rflags = r->rflags;
}

void cpu_set_current_core(uintptr_t base)
{
	asm volatile("wrmsr" : : "c"(0xc0000101), "d"((uint32_t)(base >> 32)), "a"((uint32_t)(base & 0xFFFFFFFF)));
	asm volatile("wrmsr" : : "c"(0xc0000102), "d"((uint32_t)(base >> 32)), "a"((uint32_t)(base & 0xFFFFFFFF)));
	asm volatile("swapgs");
}

bool cpu_has_apic(void)
{
    uint32_t u, eax, edx;
    __get_cpuid(1, &eax, &u, &u, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}

uint64_t cpu_get_cr3(void)
{
    uint64_t cr3;
    asm("\t mov %%cr3,%0" : "=r"(cr3));
    return cr3;
}
