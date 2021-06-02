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
 * TODO: Use kmalloc() to allocate PTs
 *
 * Paging
 */

#include "paging.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table[1024] __attribute__((aligned(4096)));
static int page_table_index = 0;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

extern void load_page_dir(uint32_t *);
extern void set_paging(void);

static void id_map(uint32_t *first_pte, uint32_t from, int size)
{
	from = from & 0xfffff000;
	for(; size>0; from += 4096, size -= 4096, first_pte++){
		*first_pte = from | 1;
	}
}

void append_page_table(uint32_t *table)
{
	/*
	 * We should just crash on real hw
	 * in case of a badly aligned PT, but just in case we do not...
	 */
	if (((uint32_t) table) % 4096 != 0) {
		panic("page table is not 4KiB aligned!");
	}
	page_directory[page_table_index++] = ((unsigned int) table) | 3;
}

void init_paging(void)
{
	for(int i = 0; i < 1024; i++) {
		/* supervisor, r&w, not present */
		page_directory[i] = 0x00000002;
	}
	id_map(page_table, 0x0, KERN_END);
	append_page_table(page_table);
	load_page_dir(page_directory);
	set_paging();
}
