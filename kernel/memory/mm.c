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
efi_mmap_t *kmmap = NULL;
size_t kmmap_size = 0;

static uint32_t total_ram = 0;
static uint32_t highest_addr = 0;
static size_t regions = 0;

/* From linker.ld */
extern uint32_t kstart;
extern uint32_t kend;

void kmem_init(efi_mmap_t *mmap)
{
	UNUSED(mmap);
	UNUSED(regions);
}

efi_memory_descriptor_t *kmem_get_kernel_mmap(void)
{
	return kmmap->map;
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
