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
 * GDT (& TSS)
 */

#include "gdt.h"

static struct tss tss = {
    .reserved = 0,
    .rsp = {},
    .reserved0 = 0,
    .ist = {},
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .iopb_offset = 0,
};

static struct gdt_ptr  gp;
static struct gdt_full gdt = {
    {0, 0, 0, 0b00000000, 0b00000000, 0},   /* Kernel NULL */
    {0, 0, 0, 0b10011010, 0b00100000, 0},   /* Kernel code */
    {0, 0, 0, 0b10010010, 0b00000000, 0},   /* Kernel data */
    {0, 0, 0, 0b11111010, 0b00100000, 0},   /* User code */
    {0, 0, 0, 0b11110010, 0b00000000, 0},   /* User data */
    {0, 0, 0, 0, 0, 0, 0, 0}                /* TSS */
};

struct gdt_tss_entry gdt_entry_tss(uintptr_t tss)
{
    return (struct gdt_tss_entry){
        .len = sizeof(struct tss),
        .base_low16 = tss & 0xffff,
        .base_mid8 = (tss >> 16) & 0xff,
        .flags1 = 0b10001001,
        .flags2 = 0,
        .base_high8 = (tss >> 24) & 0xff,
        .base_upper32 = tss >> 32,
        .reserved = 0,
    };
}

void gdt_init(void)
{
    gdt.tss = gdt_entry_tss((uintptr_t) &tss);
    gp.limit = (sizeof(gdt) - 1);
    gp.base  = (uintptr_t) &gdt;

    asm volatile (
        "lgdt %0\n"
        "push $0x08\n"
        "lea 1f(%%rip), %%rbx\n"
        "push %%rbx\n"
        "lretq\n"
        "1:\n"
        "mov %1, %%ds\n"
        "mov %1, %%es\n"
        "mov %1, %%ss\n"
        "mov %1, %%fs\n"
        "mov %1, %%gs\n"
        : : "m"(gp), "a"(__KERNEL_DS)
        : "rbx"
    );
}
