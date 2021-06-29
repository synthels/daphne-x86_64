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

#ifndef KERNEL_x86_64_TSS
#define KERNEL_x86_64_TSS

#include <stdint.h>
#include <libk/string.h>

#include "gdt.h"

/* TSS entry */
typedef struct {
	uint32_t reserved_0;
	uint64_t rsp[3];
	uint64_t reserved_1;
	uint64_t ist[7];
	uint64_t reserved_2;
	uint16_t reserved_3;
	uint16_t iomap_base;
} __attribute__ ((packed)) tss_entry_t;

/**
 * init_tss
 *   brief: Load kernel TSS
 */
void init_tss(void);

/**
 * tss_set_stack
 *   brief: Set current stack
 */
void tss_set_stack(uint64_t rsp0);

/**
 * get_tss
 *   brief: Get kernel tss
 */
tss_entry_t *get_tss(void);

#endif
