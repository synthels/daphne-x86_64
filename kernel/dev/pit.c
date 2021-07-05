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

#include "pit.h"

static uint64_t ticks = 0;

void pit_init(void)
{
	int div = 1193180 / TIMER_FREQ;
	outb(0x43, 0x34);
	outb(0x40, div & 0xFF);
	outb(0x40, div >> 8);
}

void pit_tick(void)
{
	ticks++;
}

void pit_get_ticks(uint32_t *data)
{
	*data = ticks;
}
