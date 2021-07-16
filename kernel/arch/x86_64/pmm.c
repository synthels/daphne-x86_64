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

void *pmm_alloc(size_t n)
{
    /* Start from physical address 0,
       NOT sure if this works (limine says so) */
    static size_t index = 0x0;
    size_t tmp = index;
    index += n;
    if (index > PMM_HIGH) {
        panic("pmm: out of memory!");
    }
    return (void *) tmp;
}

void pmm_free(void *ptr)
{
    UNUSED(ptr);
    /* stub */
}
