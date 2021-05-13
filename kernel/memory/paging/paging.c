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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Paging
 */

#include "paging.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table[1024] __attribute__((aligned(4096)));
static int page_table_index = 0;

extern void load_page_dir(uint32_t *);
extern void init_paging(void);

void append_page_table(uint32_t *table)
{
	for(int i = 0; i < 1024; i++) {
		/* supervisor, r&w, present */
		table[i] = (i * 0x1000) | 3;
	}
	page_directory[page_table_index++] = ((unsigned int) table) | 3;
}

void init_page_directory(void)
{
	for(int i = 0; i < 1024; i++) {
		/* supervisor, r&w, not present */
		page_directory[i] = 0x00000002;
	}
	append_page_table(page_table);
	load_page_dir(page_directory);
	init_paging();
}
