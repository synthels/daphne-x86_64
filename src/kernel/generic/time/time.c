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
 * Time & date routines
 */

#include "time.h"

static struct persistent_time_source *src;
static struct tm *info;
static uint64_t timestamp;

void time_init(void)
{
    info = src->localtime();
    timestamp = src->get_unix_timestamp();
}

void time_set_persistent_time_source(struct persistent_time_source *tms)
{
    src = tms;
}

struct tm *localtime(void)
{
    /**
     * HACK: localtime looks up the structure
     * from the time source EVERY SINGLE TIME
     */
    return src->localtime();
}

uint64_t time(void)
{
    return (timestamp + jiffies_to_sec(clock_get_jiffies()));
}

uint64_t uptime(void)
{
    return jiffies_to_sec(clock_get_jiffies());
}
