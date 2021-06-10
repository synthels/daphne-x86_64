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

extern void gdt_flush(uint32_t);
extern void tss_flush(void);

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
	gdt[num].base_high = (base >> 24) & 0xff;

	/* Limits */
	gdt[num].limit_low = (limit & 0xffff);
	gdt[num].granularity = ((limit >> 16) & 0x0f);

	/* Granularity & access flags */
	gdt[num].granularity |= (gran & 0xf0);
	gdt[num].access = access;
}

void init_gdt(void)
{
	gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gp.base = (uint32_t) &gdt;

	/* NULL seg */
	gdt_set_gate(0, 0, 0, 0, 0);

	/* kernel cs */
	gdt_set_gate(1, 0, 0xffff, 0x9a, 0xcf);

	/* kernel ds */
	gdt_set_gate(2, 0, 0xffff, 0x92, 0xcf);

	/* user cs */
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xcf);

	/* user ds */
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xcf);

	gdt_flush((uint32_t) &gp);
	tss_flush();
}
