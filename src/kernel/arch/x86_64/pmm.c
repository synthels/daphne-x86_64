/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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
 * x86_64 pmm
 */

#include "pmm.h"

declare_lock(pmm_lock);

static struct bitmap bmp;
static mmap_entry_t *memory_map = NULL;

/**
 * @brief Shrink memory map entry
 */
static void pmm_shrink(uint64_t i, size_t by)
{
    memory_map[i].base += by;
    memory_map[i].length -= by;
}

/**
 * @brief Initializes the memory bitmap
 */
void pmm_init(void)
{
    memory_map = get_memsp()->mmap;
    for (uint64_t i = 0; i < get_memsp()->size; i++) {
        if (memory_map[i].type == USABLE) {
            new_bitmap(&bmp, (void *) memory_map[i].base, memory_map[i].length);
            pmm_shrink(i, memory_map[i].length);
            break;
        }
    }
}

/**
 * @brief Adds a memory map entry and a memory address,
 * while respecting its size
 *
 * @param entry memory map entry
 * @param addr  memory address
 */
static uint64_t pmm_add(uint64_t entry, uint64_t addr)
{
    uint64_t offs = memory_map[entry].base;
    uint64_t i = entry;

    /* God, forgive me for what I'm about to do. */
    retry:
    if (offs + addr >= memory_map[i].base + memory_map[i].length + PAGE_SIZE) {
        ++i;
        goto retry;
    } else {
        return memory_map[i].base + addr;
    }
}

/**
 * @brief Allocate a whole page in physical
 * memory
 */
void *pmm_alloc(void)
{
    lock(&pmm_lock);
    int64_t free_bit = bitmap_find_free(&bmp);
    if (free_bit != -1) {
        for (uint64_t i = 0; i < get_memsp()->size; i++) {
            if (memory_map[i].type == USABLE && memory_map[i].length >= PAGE_SIZE) {
                unlock(&pmm_lock);
                bitmap_set(&bmp, free_bit, false);
                return (void *) pmm_add(i, PAGE_SIZE * free_bit);
            }
        }
    }

    unlock(&pmm_lock);
    return NULL;
}

/**
 * @brief Free a whole page of memory
 * around addr
 *
 * @param addr Valid memory address
 */
void pmm_free(void *addr)
{
    lock(&pmm_lock);

    int entry = -1;
    for (uint64_t i = 0; i < get_memsp()->size; i++) {
        if (memory_map[i].type == USABLE) {
            const uint64_t end = memory_map[i].base + memory_map[i].length;
            if (((uint64_t) addr >= memory_map[i].base) && ((uint64_t) addr <= end)) {
                entry = i;
            }
        }
    }

    /* Found correct memory map
       entry */
    if (entry >= 0) {
        bitmap_set(&bmp, (uint64_t) addr - memory_map[entry].base, false);
    }

    unlock(&pmm_lock);
}

/**
 * @brief Allocate a whole page in physical
 * memory, with proper alignment
 */
void *pmm_alloc_page(void)
{
    const size_t mask = 4096 - 1;
    const uintptr_t mem = (uintptr_t) pmm_alloc();
    return (void *) ((mem + mask) & ~mask);
}
