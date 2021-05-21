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
 *
 * PIT driver
 */

#include "PIT.h"

MODULE_NAME("pit");
MODULE_AUTH("synthels");

static uint32_t ticks = 0;
static uint32_t real_ticks = 1;

/* Initialize PIT */
void pit_init(void)
{
    int div = 1193180 / TIMER_FREQ;
    outb(0x43, 0x36);
    outb(0x40, div & 0xFF);
    outb(0x40, div >> 8);
}

/* Tick approx. every sec */
void pit_tick(void)
{
	real_ticks++;
	if (real_ticks % 96 == 0) {
		ticks++;
		real_ticks = 0;
	}
}

/* Read tick counter */
void pit_get_ticks(uint32_t *data)
{
	*data = ticks;
}
