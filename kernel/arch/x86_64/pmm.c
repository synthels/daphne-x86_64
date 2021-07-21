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
 * x64 pmm
 */

/* TODO: pmm_free? Integrate mem/malloc?? */

#include "pmm.h"

extern uint64_t kernel_end;

void *pmm_alloc(size_t n)
{
    mmap_entry_t *mmap = get_memsp()->mmap;
    for (size_t i = 0; i < get_memsp()->size; i++) {
        if (mmap[i].length >= n) {
            mmap[i].length -= n;
            mmap[i].base += n;
            return (void *) mmap[i].base;
        }
    }

    return NULL;
}

void *pmm_alloc_page(void)
{
	const size_t mask = 4096 - 1;
	const uintptr_t mem = (uintptr_t) pmm_alloc(4096 * 2);
	return (void *) ((mem + mask) & ~mask);
}

void pmm_free(void *ptr)
{
    UNUSED(ptr);
    /* stub */
}
