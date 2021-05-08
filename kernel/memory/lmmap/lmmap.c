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

#include "lmmap.h"

static uint32_t lower;

void init_lmmap(void)
{
	lower = mm_get_lower();
}

/* Linear memory map */
uint32_t *lmmap(size_t n)
{
	uint32_t *mem_ptr = (uint32_t *) lower;
	lower += n;
	return mem_ptr;
}
