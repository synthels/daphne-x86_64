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
 * i386 virtual memory manager
 */

#include "vmm.h"

static mutex_t mutex = 0;
static pte_t *page_dir;

extern void init_paging(void);
extern void load_page_dir(uint32_t *);

extern uint32_t kend;

static inline void flush_tlb_single(virtaddr_t addr)
{
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void map_page(physaddr_t physaddr, virtaddr_t virtualaddr, bool user)
{
	uint32_t pdindex = (uint32_t) virtualaddr >> 22;
	uint32_t ptindex = (uint32_t) virtualaddr >> 12 & 0x03FF;

	uint32_t *pt = &page_dir[pdindex];
	if (user) physaddr |= 0x02;
	pt[ptindex] = physaddr | 0x01 | 0x04;

	flush_tlb_single(virtualaddr);
}

void switch_address_space(physaddr_t from, physaddr_t to)
{
	acquire_mutex(&mutex);
	if ((from <= KERN_END) || (from <= to)) {
		panic("map_pages: invalid mapping");
	}

	for (uint32_t i = from; i < to; i += 4096) {
		map_page(i, i - from, true);
	}

	release_mutex(&mutex);
}

void arch_init_paging(void)
{
	for (size_t i = 0; i < 1024; i++) {
		page_dir[i] = (uint32_t) kalloc_mem_page_aligned(1024 * sizeof(uint32_t));
	}

	/* Map kernel to bottom half */
	for (uint32_t i = 0; i < KERN_END; i += 4096) {
		/* TODO: this is very obviously wrong, since I'm setting
		   the kernel's pages to be accessible by user mode.
		   This will be changed soon, with proper support
		   from pmm in order to be able to just
		   switch_address_space to the correct "user mode"
		   address space */
		map_page(i, i, true);
	}

	load_page_dir(page_dir);

	#ifndef BUILD_TESTS
	init_paging();
	#endif
}
