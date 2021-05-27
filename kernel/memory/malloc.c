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
 * Malloc
 */

/* NOTE: Free bytes don't do much as of now and kfree is useless. They might get removed soon */

#include "malloc.h"

/* Offset for each mmap entry */
static size_t mmap_offs[256];

static uint32_t *__new(size_t n)
{
	int entry_found = 0;
	size_t i = 0;
	mmap_entry_t *mmap = mm_get_kernel_mmap();
	for (; i < mmap[0].size; i++) {
		if (mmap[i].type == MEMORY_AVAILABLE) {
			/* 
			 * Check how much of this entry we
			 * have already used
			 */
			/* We add 4 because of the free byte (32 bits reserved) */
			if (mmap->length_low - mmap_offs[i] >= (n + 4)) {
				entry_found = 1;
				mmap_offs[i] += n + 4;
				break;
			}
		}
	}

	if (!entry_found)
		panic("__new: Ran out of memory!");

	return (uint32_t *) mmap[i].base_addr_low + mmap_offs[i];
}

void *kmalloc(size_t n)
{	
	/* Align n to 16 bits */
	if (n <= 16) {
		n = 16;
	} else {
		n = 16 * ((n / 16) + 1);
	}

	uint32_t *ptr = __new(n);
	/* Clear free byte */
	*ptr = 0x0;
	return (void *) ptr++;
}

void kfree(void *ptr)
{
	/* Set free byte */
	*(uint32_t *) ptr-- = 0x0;
}
