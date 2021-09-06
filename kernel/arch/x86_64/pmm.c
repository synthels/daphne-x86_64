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

/* TODO: pmm_free? Integrate malloc?? */

#include "pmm.h"
#include <dev/pcspk.h>

declare_lock(pmm_lock);

void *pmm_alloc(size_t n)
{
    lock(&pmm_lock);
    mmap_entry_t *mmap = get_memsp()->mmap;
    for (size_t i = 0; i < get_memsp()->size; i++) {
        if (mmap[i].type == USABLE) {
            if (mmap[i].length >= n) {
                mmap[i].length -= n;
                mmap[i].base += n;
                unlock(&pmm_lock);
                return (void *) mmap[i].base;
            }
        }
    }

    unlock(&pmm_lock);
    return NULL;
}

void *pmm_alloc_page(void)
{
    const size_t mask = 4096 - 1;
    const uintptr_t mem = (uintptr_t) pmm_alloc(PAGE_SIZE);
    return (void *) ((mem + mask) & ~mask);
}
