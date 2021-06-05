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
 * Watermark allocator
 */

#include "watermark.h"

/* Offset for each mmap entry */
static uint32_t mmap_offs[256];

/* wm_alloc mutex lock */
static mutex_t alloc_mutex = 0;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

static uint32_t *wm_alloc(size_t n, size_t begin)
{
	acquire_mutex(&alloc_mutex);
	mmap_entry_t *mmap = kmem_get_kernel_mmap();
	/* Go through every entry */
	for (uint32_t i = begin; i < kmem_get_kmmap_size(); i++) {
		if (mmap[i].type != MEMORY_AVAILABLE) continue;
		/* See how much of this entry we have used */
		if (mmap[i].length_low - mmap_offs[i] >= n) {
			mmap_offs[i] += n;
			release_mutex(&alloc_mutex);
			return (uint32_t *) mmap[i].base_addr_low + mmap_offs[i];
		} else {
			/* If this entry runs out, try going to the next */
			if (i < 255) {
				release_mutex(&alloc_mutex);
				return wm_alloc(n, i + 1);
			} else {
				break;
			}
		}
	}

	/* Just so gcc can shut up */
	return NULL;
}

/* We align memory to 32 bytes for performance gains with bins
   NOT because we like to waste memory (well, most of us don't) */
size_t kmem_align(size_t n)
{
	/* Align n to 32 bits */
	if (n % 32 != 0) {
		if (n <= 32) {
			n = 32;
		} else {
			n = 32 * ((n / 32) + 1);
		}
	}

	return n;
}

void *wm_alloc_mem_aligned(size_t n)
{	
	return (void *) wm_alloc(kmem_align(n), 0);
}

void *wm_alloc_mem_page_aligned(size_t n)
{
	const size_t mask = PAGE_SIZE - 1;
	const uintptr_t mem = (uintptr_t) wm_alloc(n + PAGE_SIZE, 0);
	return (void *) ((mem + mask) & ~mask);
}
