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

#include "malloc.h"

static struct malloc_ptr **heap = NULL;

void *malloc(size_t n)
{
	/* TODO: realloc all the things? */
	if (heap == NULL) {
		heap = kmalloc(256 * sizeof(struct malloc_ptr));
		for (int i = 0; i < 256; i++) {
			heap[i]->freed = 0x0;
			heap[i]->free = 0x1;
			heap[i]->size = 0;
		}
	}

	/* Search for a free pointer */
	/* TODO: very sketchy */
	struct malloc_ptr *ptr = NULL;
	for (int i = 0; i < 256; i++) {
		if (heap[i]->free) {
			ptr = heap[i];
			ptr->free = 0x0;

			/* Check if pointer is OK to reuse, without calling kmalloc again */
			/* This might be discarding alignment */
			if (heap[i]->freed && (heap[i]->size >= n)) {
				ptr->base_addr = kmalloc(n);
				ptr->size = n;
			}
		}
	}

	return (void *) ptr;
}

void free(void *ptr)
{
	struct malloc_ptr *mptr = (struct malloc_ptr *) ptr;
	mptr->freed = 0x1;
	mptr->free = 0x1;
}
