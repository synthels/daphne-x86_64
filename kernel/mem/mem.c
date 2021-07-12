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

#include "mem.h"

/* Virtual memory space */
/* Pointer to bootloader memory, might cause problems */
static memsp_t memsp;

void mem_init(void *mmap_ptr, uint64_t size)
{
    mmap_entry_t *mmap = mmap_ptr;
    /* Initialise memsp */
    memsp.size = size;
    memsp.mmap = mmap;
}

memsp_t *get_memsp()
{
    return &memsp;
}
