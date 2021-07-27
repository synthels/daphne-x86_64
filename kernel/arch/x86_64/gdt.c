/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
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
 * GDT
 */

#include "gdt.h"

static struct gdt_ptr  gp;
static struct gdt_full gdt = {
    {0, 0, 0, 0b00000000, 0b00000000, 0},   /* Kernel NULL */
    {0, 0, 0, 0b10011010, 0b00100000, 0},   /* Kernel code */
    {0, 0, 0, 0b10010010, 0b00000000, 0},   /* Kernel data */
    {0, 0, 0, 0b11111010, 0b00100000, 0},   /* User code */
    {0, 0, 0, 0b11110010, 0b00000000, 0}    /* User data */
};

void init_gdt(void)
{
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
