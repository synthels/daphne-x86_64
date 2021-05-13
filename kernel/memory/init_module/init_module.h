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

#ifndef ALLOC_MODULE
#define ALLOC_MODULE

#include <modules/module.h>
#include <logger/logk.h>
#include <memory/mm.h>
#include <kernel.h>

#define MODULE_MEM_END MODULES * sizeof(struct module_interface)

/* Initialise module */
void __mm_init_alloc_module__(void);

/* Allocates memory for a module struct */
struct module_interface *init_module(void);

/* Frees module memory */
void free_module(struct module_interface *module);

#endif
