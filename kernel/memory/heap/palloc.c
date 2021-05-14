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
 *
 * palloc
 */

#include "palloc.h"

static uint32_t palloc_ptr = MODULE_MEM_END;
static uint32_t lower;
static uint32_t upper;

void __mm_init_palloc__(void)
{
	lower = mm_get_lower();
	upper = mm_get_upper();
}

uint32_t *palloc(size_t n)
{
	/* No memory left */
	if (palloc_ptr > upper) {
		panic("ran out of memory!");
	}

	uint32_t *mem = (uint32_t *) palloc_ptr;
	palloc_ptr += n;
	return mem;
}
