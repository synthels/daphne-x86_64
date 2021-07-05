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
 * Kernel main function
 */

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

#include "kmain.h"

static uint8_t stack[STACK_SIZE] __attribute__((aligned(16)));
static struct stivale2_mmap_entry *memmap;
static uint64_t mm_size;

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
		.next = 0
	},
	.flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = (uint64_t) &terminal_hdr_tag
	},
	.framebuffer_width  = 0,
	.framebuffer_height = 0,
	.framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
	.entry_point = 0,
	.stack = (uintptr_t) stack + sizeof(stack),
	.flags = (1 << 1),
	.tags = (uintptr_t) &framebuffer_hdr_tag
};

void *get_tag(struct stivale2_struct *stv, uint64_t id)
{
	struct stivale2_tag *tag = (void *) stv->tags;
	for (;;) {
		if (tag == NULL)
			return tag;
		if (tag->identifier == id)
			return tag;
		tag = (void *) tag->next;
	}
}

void kmain(struct stivale2_struct *stv)
{
	/* Get memory map */
	struct stivale2_struct_tag_memmap *mmap = get_tag(stv, STIVALE2_STRUCT_TAG_MEMMAP_ID); /* TODO: Handle NULL */
	memmap  = mmap->memmap;
	mm_size = mmap->entries;

	init_gdt(); /* gdt & tss */
	init_idt(); /* idt */
	mem_init((void *) &memmap, mm_size); /* mm */
	dev_init(); /* essential devices */

	for (;;) {
		asm("hlt");
	}
}
