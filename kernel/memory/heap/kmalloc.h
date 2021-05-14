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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef KMALLOC
#define KMALLOC

#include <stdint.h>
#include <kernel.h>
#include "frame_tree.h"

/* Initialise module */
void __mm_init_heap__(void);

/* Allocate >= n bytes of memory */
uint32_t *kmalloc(size_t n);

#endif
