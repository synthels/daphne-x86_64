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
 * synthels' slab allocator
 *
 * This allocator works by keeping track of a linked list
 * of bins, which are essentially structures that point to
 * the head of a page in memory (All bins can have a different page size).
 * These pages also point to other pages, creating another linked list.
 * When a new page needs to be allocated, we try to find an
 * existing free page that is in a bin of the correct (or greater) size,
 * but if we don't have one, we have to create a new bin and allocate the page there
 * or allocate the page in an existing bin, if not all bins are full
 *
 * (All pages are 32 byte aligned)
 *
 * Our strategy in a diagram:
 *
 * bin -> bin -> bin -> ...
 *  \      \      \
 *   \      \    page -> page -> page -> ...
 *    \      \
 *     \     page -> page -> page -> ...
 *      \
 *      page -> page -> page -> ...
 *
 * The way kmalloc allocates pages can be seen here:
 *
 * +------------------------------+
 * | page size (uint64_t)  | page |
 * +------------------------------+
 *                         ^~~~ ptr returned
 *
 * TOmaybeDO: kmalloc() might be failing after ~500000 allocations
 * check if that's the case and if so fix urgently (possibly resolved)
 *
 * TODO: Optimize kfree (maybe store bin pointer in each page?)
 */

#include "malloc.h"

declare_lock(malloc_lock);
declare_lock(realloc_lock);
declare_lock(free_lock);

/* Allocate n bytes on the kernel heap */
uintptr_t *kalloc(size_t n)
{
    static uint64_t ap = KERNEL_HEAP_LOW;
    ap += n;
    return (uintptr_t *) (ap - n);
}

void *kalloc_mem_aligned(size_t n)
{   
    return (void *) kalloc(kmem_align(n));
}

void *kalloc_mem_page_aligned(size_t n)
{
    const size_t mask = PAGE_SIZE - 1;
    const uintptr_t mem = (uintptr_t) kalloc(n + PAGE_SIZE);
    return (void *) ((mem + mask) & ~mask);
}

static malloc_bin_t *head_bin = NULL;

/* Actual size is 0, but kmalloc doesn't bother to increment,
   so initialising to 1 is a clean solution */
static size_t hbin_size = 1;

/* Init bin (EXPECTS BIN TO BE ALLOCATED) */
void init_bin(malloc_bin_t *bin, size_t size)
{
    bin->page_size = kmem_align(size);

    /* Init first page */
    bin->first_page = kalloc_mem_aligned(sizeof(malloc_page_t));
    bin->first_page->base = kalloc_mem_aligned(size);
    bin->first_page->free = 1;

    /* Fill rest of bin */
    malloc_page_t *page = bin->first_page;
    for (int i = 0; i < MAX_PAGES - 1; i++) {
        malloc_page_t *node = kalloc_mem_aligned(sizeof(malloc_page_t));
        node->base = kalloc_mem_aligned(size);
        node->free = 1;

        /* Copy page */
        page->next_page = node;
        page = page->next_page;
    }
    page->next_page = NULL;
    bin->pages = MAX_PAGES;
}

/* Add a new bin to head_bin */
void add_bin(malloc_bin_t *bin)
{
    /* Add a new bin */
    malloc_bin_t *b = head_bin;
    for (size_t i = 0; i < hbin_size - 1; i++) {
        b = b->next_bin;
    }
    b->next_bin = bin;
    ++hbin_size;
}

/* Find a free page in a large enough bin and 
   populate it with a new page of size n */
malloc_page_t *find_free_page_and_alloc(malloc_bin_t *bin, size_t n)
{
    if ((n - 4) > bin->page_size) return NULL;
    malloc_page_t *page = bin->first_page;
    for (size_t i = 0; i < bin->pages; i++) {
        if (page->free) {
            page->free = 0;
            return page;
        }
        page = page->next_page;
    }

    return NULL;
}

/* Attempt to find a large enough existing free page and
   populate it */
malloc_page_t *find_best_bin_and_alloc(size_t n)
{
    n = kmem_align(n);
    malloc_bin_t *b = head_bin;
    malloc_page_t *page;
    for (size_t i = 0; i < hbin_size; i++) {
        /* Try to fit page in existing free page */
        if ((page = find_free_page_and_alloc(b, n)) != NULL) {
            return page;
        }
        b = b->next_bin;
    }

    return NULL;
}

/* Frees a single page from a bin */
void *free_page(malloc_bin_t *bin, void *base)
{
    malloc_page_t *page = bin->first_page;
    for (size_t i = 0; i < bin->pages; i++) {
        if ((!(page->free)) && ((page->base + 1) == base)) {
            page->free = 1;
            return page->base;
        }
        page = page->next_page;
    }
    return NULL;
}

void *kmalloc(size_t n)
{
    lock(&malloc_lock);
    /* First call, init bin */
    if (head_bin == NULL) {
        head_bin = kalloc_mem_aligned(sizeof(malloc_bin_t));
        init_bin(head_bin, n);
    }

    malloc_ptr_t *malloc_ptr;
    malloc_bin_t *bin;
    malloc_page_t *page;
    if ((page = find_best_bin_and_alloc(n)) == NULL) {
        /* We couldn't find an existing page large enough
           to accommodate our page */
        bin = kalloc_mem_aligned(sizeof(malloc_bin_t));
        init_bin(bin, n);
        add_bin(bin);
        bin->first_page->free = 0;
        /* Set malloc size right behind pointer returned */
        malloc_ptr = (malloc_ptr_t *) (bin->first_page->base);
        malloc_ptr->size = kmem_align(n);
        unlock(&malloc_lock);
        return (bin->first_page->base + 1);
    }

    page->free = 0;
    malloc_ptr = (malloc_ptr_t *) page->base;
    malloc_ptr->size = kmem_align(n);
    unlock(&malloc_lock);
    return (page->base + 1);
}

void *krealloc(void *ptr, size_t size)
{
    lock(&realloc_lock);
    void *new = kmalloc(size);
    memcpy(new, ptr, size);
    kfree(ptr);
    unlock(&realloc_lock);
    return new;
}

void *kfree(void *ptr)
{
    lock(&free_lock);
    /* Get size of allocated object */
    malloc_ptr_t *malloc_ptr = malloc_get_info(ptr);
    uint32_t malloc_size = malloc_ptr->size;
    malloc_bin_t *b = head_bin;
    void *page_base;
    for (size_t i = 0; i < hbin_size; i++) {
        /* Only bother searching bins with the
           same size as the object */
        if (b->page_size == malloc_size) {
            /* Correct bin is found */
            if ((page_base = free_page(b, ptr)) != NULL) {
                unlock(&free_lock);
                return page_base;
            }
        }
        b = b->next_bin;
    }
    unlock(&free_lock);

    /* Pointer was not allocated by kmalloc() */
    return NULL;
}
