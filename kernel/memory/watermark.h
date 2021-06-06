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
 */

#ifndef WM_ALLOC
#define WM_ALLOC

#include <stddef.h>
#include <stdint.h>
#include <logger/panic.h>
#include <memory/mm.h>
#include <mutex.h>

/* Allocate n bytes (aligns n to 32 bytes) */
void *wm_alloc_mem_aligned(size_t n);

/* Allocate n bytes (alignes ptr to page size) */
void *wm_alloc_mem_page_aligned(size_t n);

#endif
