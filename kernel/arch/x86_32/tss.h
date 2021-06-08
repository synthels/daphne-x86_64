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
 */

#ifndef KERNEL_x86_32_TSS
#define KERNEL_x86_32_TSS

#include <stdint.h>
#include <libk/string.h>

#include "gdt.h"

/* TSS entry */
struct tss_entry {
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed));

/**
 * init_tss
 *   brief: Load kernel TSS
 */
void init_tss(uint16_t ss0, uint32_t esp0);

/**
 * tss_set_stack
 *   brief: Set current stack
 *   parameters:
 *     - ss0:  new TSS ss0
 *     - esp0: new TSS esp0
 */
void tss_set_stack(uint16_t ss0, uint32_t esp0);

/**
 * get_tss
 *   brief: Get kernel tss
 */
struct tss_entry *get_tss(void);

#endif
