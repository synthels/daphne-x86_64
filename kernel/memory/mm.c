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

#include "mm.h"

static uint32_t mem_upper;
static uint32_t mem_lower;

void mm_init(uint32_t upper, uint32_t lower)
{
	mem_upper = upper;
	mem_lower = lower;

	init_lmmap();
}

uint32_t mm_get_upper()
{
	return mem_upper;
}

uint32_t mm_get_lower()
{
	return mem_lower;
}
