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

#ifndef KERNEL_x86_64_PMM
#define KERNEL_x86_64_PMM

#include <stddef.h>
#include <stdint.h>
#include <forbia/kernel.h>
#include <forbia/panic.h>

#include <libk/string.h>
#include <mem/mem.h>
#include <mem/malloc.h>

/**
 * pmm_init
 *   brief: init pmm
 */
void pmm_init(void);

/**
 * pmm_alloc
 *   brief: allocate n bytes in physical memory
 */
void *pmm_alloc(size_t n);

/**
 * pmm_alloc_page
 *   brief: allocate single page
 */
void *pmm_alloc_page(void);

#endif
