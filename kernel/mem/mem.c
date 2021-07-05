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
 * Memory manager
 */

#include "mem.h"

static mmap_entry_t *vmmap[256];
/* Virtual memory space */
static memsp_t memsp;

void mem_init(void *mmap_ptr, uint64_t size)
{
	/* Initialise memsp */
	memsp.size = 0;
	memsp.mmap = vmmap;
	/* Sanity check mmap */
	mmap_entry_t *mmap = (mmap_entry_t *) &mmap_ptr;
	for (uint64_t i = 0; i < size; i++) {
		mmap_entry_t *entry  = &mmap[i];

		/* 0 length entries */
		if (entry->length == 0x0) {
			entry->type = INVALID;
		}

		// /* Overlapping entries */
		for (uint64_t j = 0; i < memsp.size; j++) {
			/* Entry overlaps with another */
			mmap_entry_t *memspe = memsp.mmap[j];
			if (!(entry->base < memspe->base) && 
				(entry->base < (memspe->base + memspe->length))) {
				entry->type = INVALID;
			}
		}

		/* Add to memsp */
		if ((entry->type != INVALID) && (entry->type == USABLE)) {
			vmmap[memsp.size] = entry;
			++(memsp.size);
		}

		memsp.mmap = vmmap;
	}
}

memsp_t *get_memsp()
{
	return &memsp;
}
