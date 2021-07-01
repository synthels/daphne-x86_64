
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
static tss_entry_t tss;

/* Init TSS */
void init_tss(void)
{
	uintptr_t base = (uintptr_t) &tss;

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(5, base, sizeof(tss), 0xE9, 0x00);
	memset(&tss, 0x0, sizeof(tss));
	asm volatile("mov %%rsp, %0" : "=r"(tss.rsp[0]));
}

void tss_set_stack(uint64_t rsp0)
{
	tss.rsp[0] = rsp0;
}

tss_entry_t *get_tss(void)
{
	return &tss;
}
