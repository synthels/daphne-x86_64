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

#ifndef TINY_MMAP
#define TINY_MMAP

#include <stdint.h>
#include <kernel.h>
#include <memory/mm.h>

/* Init lmmap */
void init_lmmap(void);

/* Linear memory map */
uint32_t *lmmap(size_t n);

#endif
