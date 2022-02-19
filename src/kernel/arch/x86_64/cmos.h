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
#include <arch/x86_64/ports.h>

#include <lib/string.h>

#include <generic/time/persistent_time.h>
#include <generic/forbia/types.h>
#include <generic/time/time.h>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

enum cmos_data {
    CMOS_SECOND = 0,
    CMOS_MINUTE = 2,
    CMOS_HOUR = 4,
    CMOS_DAY = 7,
    CMOS_MONTH = 8,
    CMOS_YEAR = 9
};

/**
 * cmos_get_persistent_time_source
 *   brief: get cmos persistent time source
 */
struct persistent_time_source *cmos_get_persistent_time_source(void);

/**
 * cmos_to_unix
 *   brief: convert cmos time to unix timestamp
 */
uint64_t cmos_to_unix(void);

/**
 * cmos_time
 *   brief: get tm structure
 */
struct tm *cmos_time(void);
