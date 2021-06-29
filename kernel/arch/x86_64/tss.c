
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
	extern void *stack_top;
	tss.rsp[0] = (uintptr_t)&stack_top;
}

void tss_set_stack(uint16_t ss0, uint64_t rsp0)
{
	tss.rsp[0] = rsp0;
}

struct tss_entry *get_tss(void)
{
	return &tss;
}
