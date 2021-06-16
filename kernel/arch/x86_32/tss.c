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
 * TSS
 */

#include "tss.h"

/* Kernel TSS */
static struct tss_entry tss;

/* Init TSS */
void init_tss(uint16_t ss0, uint32_t esp0)
{
	uintptr_t base = (uintptr_t) &tss;

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(5, base, sizeof(tss), 0xE9, 0x00);
	memset(&tss, 0x0, sizeof(tss));
	tss.ss0 = ss0;
	tss.esp0 = esp0;
	tss.cs = 0x0b;
	tss.ss = 0x13;
	tss.ds = 0x13;
	tss.es = 0x13;
	tss.fs = 0x13;
	tss.gs = 0x13;

	tss.iomap_base = sizeof(tss);
}

void tss_set_stack(uint16_t ss0, uint32_t esp0)
{
	tss.ss0 = ss0;
	tss.esp0 = esp0;
}

struct tss_entry *get_tss(void)
{
	return &tss;
}
