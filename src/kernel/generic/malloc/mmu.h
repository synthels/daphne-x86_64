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

#pragma once

#include <stddef.h>
#include <arch/x86_64/vmm.h>

#include "malloc.h"

#define ROUND(n, m) (n + m - 1) & -m

/**
 * init_mmu
 *   brief: init mmu
 */
void init_mmu(void);

/**
 * mmu_alloc
 *   brief: allocate n bytes on the heap
 */
void *mmu_alloc(size_t n);

/**
 * mmu_free
 *   brief: free 64 byte region after ptr
 */
void mmu_free(void *ptr);
