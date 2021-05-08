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
 * Driver interface
 */

#ifndef DRIVER
#define DRIVER

#include <stdint.h>
#include <kernel.h>
#include <memory/lmmap/lmmap.h>

#define DRIVER_NO_WRITE -1

struct driver_interface {
	int (*write) (uint32_t *);
	void (*read) (uint32_t *);
	int enabled;
	int event; /* Set if an event is raised (SHOULD ONLY BE SET BY THE DRIVER & CLEARED BY THE KERNEL) */
};

#endif
