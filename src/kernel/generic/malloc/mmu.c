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

static struct bitmap *bmp = NULL;
static size_t heap_size = HEAP_PAGES * PAGE_SIZE;
static size_t heap_low = KERNEL_HEAP_LOW;

/* Allocate primitive */
static void *__alloc_primitive(size_t n)
{
    static uint64_t ap = KERNEL_HEAP_LOW;
    ap += n;
    heap_low += n;
    return (void *) (ap - n);
}

void init_mmu(void)
{
    bmp = __alloc_primitive(sizeof(struct bitmap));
    bmp->buf = __alloc_primitive((heap_size / 64) * sizeof(char));
    memset(bmp->buf, 0xff, heap_size / 64);
}

/* Does the dirty allocation work, also needs n to be a multiple of 64!! */
void *__mmu_alloc(size_t n)
{
    for (uint64_t region = heap_low; region < heap_low + heap_size; region += 64) {
        /* Bitmap index */
        uint64_t i = (region - heap_low) / 64;
        if (bitmap_get(bmp, i)) {
            /* Check all regions needed for allocation */
            for (size_t j = i; j <= i + (n / 64); j++) {
                if (!bitmap_get(bmp, j)) {
                    break;
                } else if ((j - i) < (n / 64)) {
                    /* Yes, this wastes memory if an allocation
                       overflows the heap. But at that point, does
                       it even matter??? */
                    bitmap_set(bmp, j, 0);
                } else {
                    return (void *) region;
                }
            }
        }
    }
    return NULL;
}

void *mmu_alloc(size_t n)
{
    return __mmu_alloc(ROUND(n, 64));
}

void mmu_free(void *ptr)
{
    /* FIXME: ignores the alignment of ptr, while it should strictly be
              a multiple of 64! */
    if ((uint64_t) ptr > heap_low + heap_size){
        panic("mmu_free: invalid pointer!");
    }
    bitmap_set(bmp, (uint64_t) (ptr - heap_low) / 64, 1);
}

void mmu_map_mmio(uintptr_t base, size_t pages)
{
    for (int i = 0; i < pages; i++) {
        pml4_map_page(base + i * PAGE_SIZE, base + i * PAGE_SIZE, FLAGS_READ_WRITE);
    }
}

context_t *mmu_init_context(size_t heap, uint64_t stack)
{
    return (context_t *) init_context(heap, stack);
}

uint64_t *mmu_vmalloc(size_t n)
{
    return vmalloc(n);
}

void mmu_vswitch(uint64_t *pml)
{
    vswitch(pml);
}

void mmu_switch(context_t *context)
{
    mmu_vswitch(context->page_table);
    arch_swapregs(context->regs);
}
