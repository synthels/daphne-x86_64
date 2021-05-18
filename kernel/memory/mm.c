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

#include "mm.h"

static mmap_entry_t *mmap_begin;

void mm_init(mmap_entry_t *mmap_addr, uint32_t length)
{
	uint32_t total_ram = 0;
	size_t regions = 0;
	mmap_entry_t *mmap = mmap_addr;
	mmap_begin = mmap_addr;
	/* Process mmap */
	for (size_t i = 0; mmap < (mmap_addr + length); i++) {
		/* Mark "invalid" entries */
		if ((regions > 0 && mmap->base_addr_low == 0x0) || mmap->length_low == 0x0) {
			mmap->type = MEMORY_INVALID;
		}

		/* Print region info */
		switch (mmap->type) {
			case MEMORY_AVAILABLE:
				printk("[available memory] starting from: 0x%ux, with a length of %uiB", mmap->base_addr_low, mmap->length_low);
				total_ram += mmap->length_low;
				regions++;
				break;
			case MEMORY_RESERVED:
				printk("[reserved memory] starting from: 0x%ux, with a length of %uiB", mmap->base_addr_low, mmap->length_low);
				break;
			case MEMORY_ACPI:
				printk("[acpi] starting from: 0x%ux, with a length of %uiB", mmap->base_addr_low, mmap->length_low);
				break;
			case MEMORY_NVS:
				printk("[acpi nvs] starting from: 0x%ux, with a length of %uiB", mmap->base_addr_low, mmap->length_low);
				break;
			case MEMORY_BADRAM:
				printk("[bad] starting from: 0x%ux, with a length of %uiB", mmap->base_addr_low, mmap->length_low);
				break;
			case MEMORY_INVALID:
				break;
		}

		/* Next entry */
		mmap = (mmap_entry_t *) ((uint32_t) mmap + mmap->size + sizeof(mmap->size));
	}

	printk("[mm] total available memory: %uiB, %i available regions detected", total_ram, regions);
}

mmap_entry_t *mm_get_mmap()
{
	return mmap_begin;
}
