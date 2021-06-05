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
 * TODO: Recursive mapping?
 *
 * Paging
 */

#include "paging.h"

static pdir_t *page_directory;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

extern void load_page_dir(uint32_t *);
extern void enter_paging(void);

static void id_map(uint32_t *first_pte, uint32_t from, int size)
{
	from = from & 0xfffff000;
	for(; size > 0; from += 4096, size -= 4096, first_pte++){
		*first_pte = from | 1;
	}
}

void add_pte(uint32_t *table)
{
	static int pte_index = 0;
	if (((uint32_t) table) % 4096 != 0) {
		panic("page table is not 4KiB aligned!");
	}
	page_directory[pte_index++] = ((unsigned int) table) | 3;
}

void kbrk(size_t n)
{
	static uint32_t paged_high = 0;
	pte_t *page_table = wm_alloc_mem_page_aligned(1024);
	id_map(page_table, paged_high, paged_high + n);
	add_pte(page_table);
	paged_high += n;
}

void init_paging(void)
{
	/* Allocate page dir */
	page_directory = wm_alloc_mem_page_aligned(1024);
	for(int i = 0; i < 1024; i++) {
		/* supervisor, r&w, not present */
		page_directory[i] = 0x00000002;
	}

	/* Extend memory space just enough so that we don't crash */
	kbrk(KERN_END);
	load_page_dir(page_directory);
	enter_paging();
}
