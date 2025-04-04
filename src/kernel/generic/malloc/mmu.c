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
 * Primitive virtual memory manager
 *
 * Note 1: mmu_allocate works by splitting the heap in 64 byte chunks
 * (where 64 is a VERY arbitrary choice btw) and so all allocated memory
 * sizes are rounded up to a mulitple of 64.
 */

#include "mmu.h"

/**
 * @brief Allocate n bytes in the heap
 *
 * Watermarks n bytes
 */
void *mmu_alloc(size_t n)
{
    static uint64_t ap = KERNEL_HEAP_LOW;
    static size_t heap_size = HEAP_PAGES * PAGE_SIZE;
    if (ap + n < KERNEL_HEAP_LOW + heap_size) {
        ap += n;
        return (void *) (ap - n);
    } else {
        panic("out of memory!");
    }
    __builtin_unreachable();
}

/**
 * @brief Memory map in the current address space
 *
 * @param base Base address
 * @param pages Number of pages
 */
void mmap_current(uintptr_t base, size_t pages)
{
    for (size_t i = 0; i < pages; i++) {
        pml4_map_page(base + i * PAGE_SIZE, base + i * PAGE_SIZE, KERNEL_PAGE_ACCESS);
    }
}

/**
 * @brief Memory map
 *
 * Identity maps a range in physical memory
 *
 * @param pml Virtual address space
 * @param base Base address
 * @param pages Number of pages
 */
void mmap(void *pml, uintptr_t base, size_t pages)
{
    for (size_t i = 0; i < pages; i++) {
        map_page(pml, base + i * PAGE_SIZE, base + i * PAGE_SIZE, KERNEL_PAGE_ACCESS);
    }
}

/**
 * @brief Virtual map
 *
 * Allocates physical memory and maps it to specified
 * virtual memory
 *
 * @param pml Virtual address space
 * @param base Base address
 * @param pages Number of pages
 */
void user_virt_map(void *pml, uintptr_t base, size_t pages)
{
    for (size_t i = 0; i < pages; i++) {
        map_page(pml, base + i * PAGE_SIZE, (uintptr_t) pmm_alloc_page(), USER_PAGE_ACCESS);
    }
}

/**
 * @brief Initialise process context
 *
 * @param heap Heap base address
 * @param stack Stack base address
 */
struct context *mmu_init_context(void)
{
    return (struct context *) init_context();
}

/**
 * @brief Switch to process context
 *
 * @param context Process context
 */
void mmu_swap_context(struct context *context)
{
    swap_page_directory(context->page_table);
}
