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

/* mmap as given by grub */
static mmap_entry_t *mmap_begin;

/* mmap as sanity checked by the kernel */
static mmap_entry_t kmmap[256];
static size_t kmmap_size = 0;
static uint32_t mmap_length = 0;

/* RAM info */
static uint32_t total_ram = 0;
static uint32_t highest_addr = 0;
static size_t regions = 0;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

/* Paging */
extern void init_paging(void);
extern void load_page_dir(uint32_t *);
static pdir_t *page_directory;

/* Dump mmap entry */
void dump_entry(mmap_entry_t *entry)
{
	switch (entry->type) {
		case MEMORY_AVAILABLE:
			printk("base_addr=%ui, length=%uiB - available", entry->base_addr_low, entry->length_low);
			break;
		case MEMORY_RESERVED:
			printk("base_addr=%ui, length=%uiB - reserved", entry->base_addr_low, entry->length_low);
			break;
		case MEMORY_NVS:
		case MEMORY_ACPI:
			printk("base_addr=%ui, length=%uiB - acpi", entry->base_addr_low, entry->length_low);
			break;
	}
}

void kmem_init(multiboot_info_t *info)
{
	mmap_entry_t *mmap = (mmap_entry_t *) info->mmap_addr;
	mmap_begin = (mmap_entry_t *) info->mmap_addr;
	mmap_length = info->mmap_length;

	/* Validate mmap */
	for (size_t i = 0; (uint32_t) mmap < (info->mmap_addr + info->mmap_length); i++) {
		/* 0 length entries */
		if (mmap->length_low == 0x0) {
			mmap->type = MEMORY_INVALID;
		}

		/* Entry overlaps with kernel code */
		if (mmap->base_addr_low + mmap->length_low <= KERN_END) {
			mmap->type = MEMORY_INVALID;
		} else if ((mmap->base_addr_low <= KERN_END) && mmap->type == MEMORY_AVAILABLE) {
			/* If only a part of it does, keep the part that doesn't */
			mmap->base_addr_low += KERN_END + 1;
			mmap->length_low -= KERN_END + 1;
		}

		/* Overlapping entries */
		for (size_t j = 0; i < kmmap_size; j++) {
			/* Entry overlaps with another */
			if (!(mmap->base_addr_low < kmmap[j].base_addr_low) && 
				(mmap->base_addr_low < (kmmap[j].base_addr_low + kmmap[j].length_low))) {
				mmap->type = MEMORY_INVALID;
			}
		}

		/* Append entry to kmmap */
		if (mmap->type != MEMORY_INVALID) {
			if (mmap->type == MEMORY_AVAILABLE) {
				regions++;
			}
			kmmap[kmmap_size] = *mmap;
			++kmmap_size;
		}

		total_ram += mmap->length_low;
		dump_entry(mmap);
		/* Next entry */
		mmap = (mmap_entry_t *) ((uint32_t) mmap + mmap->size + sizeof(mmap->size));
	}

	/* You expect this shit to run with less than 5clmb ram? */
	if (total_ram <= MiB(5)) {
		panic("Here's a nickel kid. Go buy yourself a real computer");
	}
}

void add_pte(uint32_t *table)
{
	static int pte_index = 0;
	if (((uint32_t) table) % 4096 != 0) {
		panic("page table is not 4KiB aligned!");
	}
	
	page_directory[pte_index++] = ((uint32_t) table) | 3;
}

void kmem_map_pages(uint32_t *first_pte, uint32_t from, int size)
{
	/* Map pages */
	from = from & 0xfffff000;
	for(; size > 0; from += 4096, size -= 4096, first_pte++){
		*first_pte = from | 1;
	}
}

uint32_t kmem_extend_address_space_4mib(size_t n)
{
	static uint32_t paged_high = 0;
	/* Allocate new table */
	pte_t *table = wm_alloc_mem_page_aligned(1024);
	/* Map pages as needed */
	kmem_map_pages(table, paged_high, paged_high + n);
	add_pte(table);
	paged_high += n;
	return paged_high;
}

void kmem_extend_address_space(size_t n)
{
	const size_t fmb = MiB(4);
	/* If size is below 4MiB, only extend
	   by the size */
	if (n <= fmb) {
		kmem_extend_address_space_4mib(n);
		return;
	}
	/* Get number of pages we need to allocate */
	size_t div = fast_ceil(n, fmb);
	for (size_t i = 0; i < div; i++) {
		kmem_extend_address_space_4mib(fmb);
	}
}

void kmem_init_paging(void)
{
	/* Allocate page dir */
	page_directory = wm_alloc_mem_page_aligned(1024);
	for(int i = 0; i < 1024; i++) {
		/* supervisor, r&w, not present */
		page_directory[i] = 0x00000002;
	}

	/* Extend just enough so that we don't crash */
	kern_queue_calls(kmem_extend_address_space(MiB(4)), kmem_extend_address_space(KERN_END), KERN_END <= MiB(4));
	load_page_dir(page_directory);
	init_paging();
}

/**************************/
/**************************/

mmap_entry_t *kmem_get_kernel_mmap()
{
	return kmmap;
}

size_t kmem_get_kmmap_size()
{
	return kmmap_size;
}

uint32_t kmem_get_installed_memory()
{
	return total_ram;
}

uint32_t kmem_get_highest_addr()
{
	return highest_addr;
}
