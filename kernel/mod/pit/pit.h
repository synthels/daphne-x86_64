
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
 */

#pragma once

#include <io/io.h>
#include <forbia/kernel.h>

#define TIMER_FREQ 100

/**
 * pit_init
 *   brief: init pit
 */
void pit_init(void);

/**
 * pit_tick
 *   brief: do one tick
 */
void pit_tick(void);

/**
 * pit_get_ticks
 *   brief: get current tick count
 */
void pit_get_ticks(uint32_t *data);
