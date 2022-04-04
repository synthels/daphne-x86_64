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
 *
 * Note 2: mmu_free frees the 64 byte chunk that ptr resides in,
 * but it doesn't go further than that.
 */

#include "mmu.h"

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

void mmap_file(uintptr_t base, size_t pages)
{
    for (size_t i = 0; i < pages; i++) {
        pml4_map_page(base + i * PAGE_SIZE, base + i * PAGE_SIZE, FLAGS_READ_WRITE);
    }
}

struct context *mmu_initcontext(size_t heap, uint64_t stack)
{
    return (struct context *) initcontext(heap, stack);
}

uint64_t *mmu_vmalloc(size_t n)
{
    return vmalloc(n);
}

void mmu_vswitch(uint64_t *pml)
{
    vswitch(pml);
}

void mmu_switch(struct context *context)
{
    mmu_vswitch(context->page_table);
}
