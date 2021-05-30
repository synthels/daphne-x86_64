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
 * (All pages are 32 bit aligned)
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
 * | page size (uint32_t)  | page |
 * +------------------------------+
 *                         ^~~~ ptr returned
 *
 * TODO: kmalloc() might be failing after ~500000 allocations
 * check if that's the case and if so fix urgently
 */

#include "malloc.h"

static malloc_bin_t *head_bin = NULL;
static size_t hbin_size = 0;

/* Init bin EXPECTS BIN TO BE ALLOCATED */
void init_bin(malloc_bin_t *bin, size_t size)
{
	bin->page_size = kmem_align(size);

	/* Fill bin */
	malloc_page_t *page = bin->first_page;
	for (int i = 0; i < MAX_PAGES - 1; i++) {
		page = alloc_mem_aligned(sizeof(malloc_page_t));
		page->base = alloc_mem_aligned(size); /* Allocate page */
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
		b = head_bin->next_bin;
	}
	b->next_bin = bin;
	++hbin_size;
}

/* Find a free page in a large enough bin and 
   populate it with a new page of size n */
malloc_page_t *find_free_page_and_alloc(malloc_bin_t *bin, size_t n)
{
	if (bin->page_size < n) return NULL;
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

/* Attempt to add page to bin */
malloc_page_t *add_page(malloc_bin_t *bin, size_t n)
{
	/* Silently ignore the deafening screams of the kernel */
	if (bin->pages >= MAX_PAGES) return NULL;
	if (bin->page_size != (n - 32)) return NULL;

	malloc_page_t *page = bin->first_page;
	for (size_t i = 0; i < bin->pages - 1; i++) {
		page = page->next_page;
	}

	page->next_page = alloc_mem_aligned(n);
	return page;
}

/* Attempt to find non full bin and add page there,
   or find a large enough existing free page and
   populate it */
malloc_page_t *find_best_bin_and_alloc(size_t n)
{
	/* Add 32 bits to n */
	n = kmem_align(n) + 32;
	malloc_bin_t *b = head_bin;
	malloc_page_t *page;
	for (size_t i = 0; i < hbin_size; i++) {
		/* Whether or not bin is full, try to fit page
		   in existing free page */
		if ((page = find_free_page_and_alloc(b, n)) == NULL) {
			/* If we can't and bin is not full, add page */
			if (b->pages < MAX_PAGES) {
				return add_page(b, n);
			}
		} else {
			return page;
		}
		b = head_bin->next_bin;
	}

	return NULL;
}

void *kmalloc(size_t n)
{
	/* First call, init bin */
	if (head_bin == NULL) {
		head_bin = alloc_mem_aligned(sizeof(malloc_bin_t));
		init_bin(head_bin, n);
	}

	malloc_bin_t *bin;
	malloc_page_t *page;
	if ((page = find_best_bin_and_alloc(n)) == NULL) {
		/* We couldn't find a bin to add our page to
		   nor could we find an existing page large enough
		   to accommodate our page */
		bin = alloc_mem_aligned(sizeof(malloc_bin_t));
		init_bin(bin, n);
		add_bin(bin);
		return bin->first_page->base;
	}

	/* TODO: memory checks */
	/* TODO: include malloc info to page */
	return page->base;
}

void kfree(void *ptr)
{
	UNUSED(ptr);
	/* TODO: implement kfree */
}
