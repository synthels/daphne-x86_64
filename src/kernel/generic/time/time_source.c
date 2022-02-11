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
 * Time sources
 */

#include "time_source.h"

static enum time_source best_time_source = TIME_SOURCE_NONE;

void time_source_set(enum time_source t)
{
    best_time_source = t;
}

int time_source_get_jiffies(void)
{
    switch(best_time_source) {
        case TIME_SOURCE_PIT:
            return pit_get_jiffies();
        case TIME_SOURCE_APIC:
            /* TODO */
            break;
        case TIME_SOURCE_HPET:
            return hpet_get_jiffies();
        default:
            panic("no time source available!");
    }
    __builtin_unreachable();
}
