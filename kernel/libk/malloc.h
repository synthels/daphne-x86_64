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

#include <memory/malloc.h>
#include <stddef.h>
#include <stdint.h>

struct malloc_ptr {
	void *base_addr;
	uint8_t free;
	/* 
	 * Set by free(). Used to differentiate between
	 * pointers that have been allocated and
	 * freed from pointers that have not been
	 * allocated at all. Essentialy, it being
	 * set means that base_addr can be trusted
	 */
	uint8_t freed;
	uint32_t size;
} __attribute__((packed));

/* Attempt to allocate n bytes (4 byte aligned) */
/* Returns NULL if it fails */
void *malloc(size_t n);

/* Frees a pointer returned by malloc */
void free(void *ptr);

#endif
