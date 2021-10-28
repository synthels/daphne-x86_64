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

#ifndef KERNEL_SPACE
#define KERNEL_SPACE

#include "process.h"

#ifdef ARCH_x86_64
    #include <arch/x86_64/vmm.h>
#endif

/**
 * generic_vmalloc
 *   brief: Create new virtual address space of size n
 */
uint64_t *generic_vmalloc(size_t n);

#endif
