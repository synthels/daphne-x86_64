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

static struct gdt_entry gdt[6];
static struct gdt_ptr gp;

void gdt_set_gate(
	int num,
	uint32_t base,
	uint32_t limit,
	uint8_t access,
	uint8_t gran)
{
	/* Base address */
	gdt[num].base_low = (base & 0xffff);
	gdt[num].base_middle = (base >> 16) & 0xff;
	gdt[num].base_high = (base >> 24 & 0xff);

	/* Limits */
	gdt[num].limit_low = (limit & 0xffff);
	gdt[num].granularity = (limit >> 16) & 0x0f;

	/* Granularity & access flags */
	gdt[num].granularity |= (gran & 0xf0);
	gdt[num].access = access;
}

void init_gdt(void)
{
	gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gp.base = (uint64_t) &gdt; /* uintptr_t?? */

	/* NULL seg */
	gdt_set_gate(0, 0, 0, 0, 0);
	/* kernel cs */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* kernel ds */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	/* user cs */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	/* user ds */
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	asm volatile (
		"mov %0, %%rdi\n"
		"lgdt (%%rdi)\n"
		"mov $0x10, %%ax\n"
		"mov %%ax, %%ds\n"
		"mov %%ax, %%es\n"
		"mov %%ax, %%ss\n"
		"mov $0x2b, %%ax\n"
		"ltr %%ax\n"
		: : "r"(&gp)
	);

	/* tss_flush(); */
}
