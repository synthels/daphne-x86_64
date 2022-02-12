/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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
 * PIT
 */

#include "pit.h"

void pit_init(void)
{
    int div = 1193180 / HZ;
    outb(0x43, 0x34);
    outb(0x40, div & 0xFF);
    outb(0x40, div >> 8);
    time_source_set(TIME_SOURCE_PIT);
}
