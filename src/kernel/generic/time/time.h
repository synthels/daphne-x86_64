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
 */

#pragma once

#include <stdint.h>

#include "persistent_time.h"
#include "jiffies.h"
#include "clock.h"
#include "tm.h"

/**
 * time_init
 *   brief: init time module
 */
void time_init(void);

/**
 * time_set_persistent_time_source
 *   brief: set persistent time source
 */
void time_set_persistent_time_source(struct persistent_time_source *tms);

/**
 * localtime
 *   brief: get local time
 */
struct tm *localtime(void);

/**
 * time
 *   brief: get seconds since the epoch
 */
uint64_t time(void);

/**
 * uptime
 *   brief: get seconds since boot
 */
uint64_t uptime(void);
