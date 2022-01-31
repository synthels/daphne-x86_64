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
 * Sleep function
 */

#include "sleep.h"

void sleep(uint32_t ticks)
{
    uint32_t start;
    uint32_t now;
    start = pit_get_ticks();
    now = pit_get_ticks();
    while ((now - start) < ticks)
        now = pit_get_ticks();
}
