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
 * Memory manager (stubs)
 */

#include "mm.h"

/* RAM info */
efi_memory_descriptor_t kmmap[255];
size_t kmmap_size = 0;

static uint32_t total_ram = 0;
static uint32_t highest_addr = 0;
static size_t regions = 0;

/* linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

void kmem_init(efi_mmap_t *mmap)
{
	/* Assuming that UEFI map is relatively sane */
	efi_memory_descriptor_t *mement;
	for(mement = mmap->map; (uint8_t *) mement < (uint8_t *) mmap->map + mmap->size; mement = next_desc(mement, mmap->desc_size)) {
		/* Entry overlaps with kernel code */
		if (mement->phys_start + (mement->pages * 4096) <= KERN_END) {
			mement->type = MEMORY_INVALID;
		} else if ((mement->phys_start <= KERN_END) && mement->type == MEMORY_AVAILABLE) {
			/* If only a part of it does, keep the part that doesn't */
			mement->phys_start += KERN_END + 1;
			mement->pages -= (KERN_END / 4096) + 1;
		}

		/* Append entry to kmmap */
		if (mement->type != MEMORY_INVALID) {
			if (mement->type == MEMORY_AVAILABLE) {
				regions++;
			}
			kmmap[kmmap_size] = *mement;
			++kmmap_size;
		}
	}
}

efi_memory_descriptor_t *kmem_get_kernel_mmap(void)
{
	return kmmap;
}

size_t kmem_get_kmmap_size(void)
{
	return kmmap_size;
}

uint32_t kmem_get_installed_memory(void)
{
	return total_ram;
}

uint32_t kmem_get_highest_addr(void)
{
	return highest_addr;
}
