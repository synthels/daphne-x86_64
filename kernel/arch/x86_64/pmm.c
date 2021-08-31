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

declare_lock(pmm_lock);

static bool PMM_ALLOC_FIRST_CALL = true;

void *pmm_alloc(size_t n)
{
    lock(&pmm_lock);
    mmap_entry_t *mmap = get_memsp()->mmap;
    static int last_index = -1;
    int i = 0;
    for (; i < (int) get_memsp()->size; i++) {
        if (mmap[i].type == USABLE) {
            uint64_t len = mmap[i].length;
            volatile uint64_t *base = (uint64_t *) mmap[i].base;
            /* check if we used a new entry this time
               so that we don't screw with the entry's
               length */
            if ((i > last_index) || PMM_ALLOC_FIRST_CALL) {
                *base = len;
                PMM_ALLOC_FIRST_CALL = false;
            }
            if (*base >= n) {
                *(base) -= n;
                last_index = i;
                unlock(&pmm_lock);
                return (void *) ((*base - n) + (mmap[i].base + BASE_OFFS));
            }
        }
    }

    last_index = i;
    unlock(&pmm_lock);
    return NULL;
}

void *pmm_alloc_page(void)
{
    const size_t mask = 4096 - 1;
    const uintptr_t mem = (uintptr_t) pmm_alloc(4096 * 2);
    return (void *) ((mem + mask) & ~mask);
}
