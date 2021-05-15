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
 * kmalloc
 */

#include "kmalloc.h"

static uint32_t lower;
static uint32_t upper;
static uint32_t map_mem_start;
static uint32_t usable_mem_end;

size_t kbrk_index = 0;

void __mm_init_heap__(void)
{
	lower = mm_get_lower();
	upper = mm_get_upper();

	/* It's the start but whatever */
	usable_mem_end = lower + (MAX_TREES * sizeof(struct frame_tree_node) * NODES_PER_FRAME);
	map_mem_start = lower + MODULE_MEM_END;
}

uint32_t *kmalloc(size_t n)
{
	UNUSED(n);
	return NULL;
}

void kmap()
{
	if (kbrk_index >= MAX_TREES) {
		logk("memory/kmap: cannot map memory any further");
		return;
	}

	/* Allocate new tree */
	struct frame_tree_node *tree;
	ALLOC_NODE(tree);

	/* Move usable_mem_end & map */
	tmap(usable_mem_end, lower, upper, tree);
	usable_mem_end += 512;

	/* Add tree to map */
	kbrk_index++;
}

uint32_t *kbrk(size_t n)
{
	/* n not a multiple of 512 */
	if (n % 512 != 0) {
		panic("kbrk can only extend memory by a factor multiple of 512");
	}

	/* Break up call to multiple kmap calls */
	for (size_t i = 0; i < n / 512; i++) {
		kmap();
	}

	return (uint32_t *) usable_mem_end;
}
