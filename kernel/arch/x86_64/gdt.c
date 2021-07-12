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

static struct gdt_ptr gp;
static struct gdt_desc gdt;

void gdt_set_desc(uint8_t n, uint16_t lim, uint16_t base_low, uint8_t base_mid, uint8_t base_high, uint8_t access, uint8_t gran)
{
    gdt.entries[n].limit = lim;
    gdt.entries[n].base_low = base_low;
    gdt.entries[n].base_mid = base_mid;
    gdt.entries[n].access = access;
    gdt.entries[n].granularity = gran;
    gdt.entries[n].base_hi = base_high;
}

void init_gdt(void)
{
    /* Set up gdt */
    gdt_set_desc(0, 0, 0, 0, 0, 0, 0);
    gdt_set_desc(1, 0xffff, 0x0000, 0x00, 0x00, 0b10011010, 0b00000000);
    gdt_set_desc(2, 0xffff, 0x0000, 0x00, 0x00, 0b10010010, 0b00000000);
    gdt_set_desc(3, 0xffff, 0x0000, 0x00, 0x00, 0b10011010, 0b11001111);
    gdt_set_desc(4, 0xffff, 0x0000, 0x00, 0x00, 0b10010010, 0b10011010);

    /* Set up tss */
    gdt.tss.length       = 104;
    gdt.tss.base_low16   = 0;
    gdt.tss.base_mid8    = 0;
    gdt.tss.flags1       = 0b10001001;
    gdt.tss.flags2       = 0;
    gdt.tss.base_high8   = 0;
    gdt.tss.base_upper32 = 0;
    gdt.tss.reserved     = 0;

    gp.limit = sizeof(gdt) - 1;
    gp.base = (uintptr_t) &gdt;

    /* Load 'em both */
    asm volatile (
        "mov %0, %%rdi\n"
        "lgdt (%%rdi)\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov $0x2b, %%ax\n"
        "ltr %%ax\n"
        : : "r"((uintptr_t) &gp)
    );
}
