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

struct tm {
    int tm_sec;     /* seconds (0-59) */
    int tm_min;     /* minutes (0-59) */
    int tm_hour;    /* hours (0-23) */
    int tm_mday;    /* day of the month (1-31) */
    int tm_mon;     /* month (0-11) */
    int tm_year;    /* The number of years since 2000 */
    int tm_wday;    /* day of the week (0-6) */
    int tm_yday;    /* day in the year (0-365) */
    int tm_isdst;   /* daylight saving time */
};
