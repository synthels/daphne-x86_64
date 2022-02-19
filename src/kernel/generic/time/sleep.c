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

void delay(uint64_t jiff)
{
    uint32_t start;
    uint32_t now;
    start = time_source_get_jiffies();
    now = time_source_get_jiffies();
    while ((now - start) < jiff)
        now = time_source_get_jiffies();
}

void sleep(int sec)
{
    delay(sec_to_jiffies(sec));
}

void nanosleep(int usec)
{
    delay(usec_to_jiffies(usec));
}

void microsleep(int msec)
{
    delay(msec_to_jiffies(msec));
}
