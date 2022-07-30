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
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include <generic/forbia/kernel.h>
#include <lib/panic.h>
#include <lib/lock.h>

#include <lib/string.h>
#include <generic/memory/mem.h>
#include <generic/memory/bitmap.h>
#include <generic/malloc/malloc.h>

#define BASE_OFFS 1

/**
 * pmm_init
 *   brief: init pmm
 */
void pmm_init(void);

/**
 * pmm_alloc
 *   brief: allocate PAGE_SIZE bytes in physical memory
 */
void *pmm_alloc(void);

/**
 * pmm_free
 *   brief: Free a whole page of memory around addr
 */
void pmm_free(void *addr);

/**
 * pmm_alloc_page
 *   brief: allocate single page
 */
void *pmm_alloc_page(void);
