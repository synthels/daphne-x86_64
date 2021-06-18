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

#ifndef KERNEL_x86_32_VMM
#define KERNEL_x86_32_VMM

#include <stdint.h>

typedef uint32_t pte_t;
typedef uint32_t pdir_t;

/**
 * arch_init_paging
 *   brief: init paging on i386
 */
void arch_init_paging(void);

#endif
