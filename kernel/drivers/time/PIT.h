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

#ifndef DRIVER_PIT
#define DRIVER_PIT

#include <io/io.h>
#include <drivers/driver.h>
#include <stdint.h>

#define TIMER_FREQ 100

struct driver_interface *pit_interface;

/* Initialize PIT */
void pit_init(void);

/* Tick approx. every sec */
void pit_tick(void);

/* Read tick counter */
void pit_get_ticks(uint32_t *data);

#endif
