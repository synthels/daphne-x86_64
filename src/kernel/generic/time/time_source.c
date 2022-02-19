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
 * Controls time IRQ sources
 */

#include "time_source.h"

static enum time_source best_time_source = TIME_SOURCE_NONE;
static uint64_t jiffies = 0;

void time_source_set(enum time_source t)
{
    best_time_source = t;
}

void time_source_tick(regs_t *r)
{
    ++jiffies;
    clock_run_hooks(r, jiffies);
}

void time_source_irq_handler(regs_t *r)
{
    time_source_tick(r);
}

int time_source_get_jiffies(void)
{
    return jiffies;
}
