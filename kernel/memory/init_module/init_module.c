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
 * Module init
 */

#include "init_module.h"

static uint32_t lower;

void __mm_init_alloc_module__(void)
{
	lower = mm_get_lower();
}

struct module_interface *init_module(void)
{
	struct module_interface *mem_ptr = (struct module_interface *) lower;
	lower += sizeof(struct module_interface);
	return mem_ptr;
}
