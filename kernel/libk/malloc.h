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
 * Malloc
 */

#ifndef LIBK_MALLOC
#define LIBK_MALLOC

#include <memory/alloc.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel.h>

#define MAX_PAGES 12 /* Max pages in bin */

struct malloc_page {
	struct malloc_page *next_page; /* NULL if last page */
	size_t free;
	uint32_t *base;
};

typedef struct malloc_page malloc_page_t;

struct malloc_bin {
	struct malloc_bin *next_bin; /* NULL if last node */
	struct malloc_page *first_page;
	size_t page_size; /* This bin's page size */
	size_t pages; /* Number of pages in bin */
};

typedef struct malloc_bin malloc_bin_t;

/* Attempt to allocate n bytes (4 byte aligned) */
/* Returns NULL if it fails */
void *kmalloc(size_t n);

/* Frees a pointer returned by malloc */
void kfree(void *ptr);

#endif
