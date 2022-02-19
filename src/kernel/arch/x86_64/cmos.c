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
 * CMOS routines
 */

#include "cmos.h"

#define bcd_to_bin(val) ((val / 16) * 10 + (val & 0xf))

static uint16_t data[128];
static uint16_t prev_data[128];

static struct persistent_time_source cmos = {
    .get_unix_timestamp = cmos_to_unix,
    .localtime = cmos_time
};

struct persistent_time_source *cmos_get_persistent_time_source(void)
{
    return &cmos;
}

static inline bool is_leap_year(int year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/**
 * @brief Check if the cmos is updating
 */
static bool is_cmos_updating(void)
{
    outb(CMOS_ADDRESS, 0x0A);
    return (inb(CMOS_DATA) & 0x80);
}

static void cmos_dump(uint16_t *values)
{
    for (uint16_t i = 0; i < 128; ++i) {
        outb(CMOS_ADDRESS, i);
        values[i] = inb(CMOS_DATA);
    }
}

static uint64_t cmos_get(enum cmos_data data, uint16_t *val)
{
    return bcd_to_bin(val[data]);
}

static void cmos_update(void)
{
    while (is_cmos_updating());
    cmos_dump(data);
}

static void leap_years_correction(uint64_t *days, int years)
{
    if (years % 4) return;
    else if (years % 100) (*days) += 1;
    else if (years % 400) return;
    else (*days) += 1;
}

/**
 * @brief Seconds since the unix epoch
 *
 * @param year years since 2000
 */
static uint64_t secs_since_epoch(int year)
{
    uint64_t days = 0;
    year += 2000;
    while (year >= 1970) {
        days += 365;
        leap_years_correction(&days, year);
        --year;
    }
    return days * 86400;
}

/**
 * @brief get number of seconds in month
 *
 * No.  Month     Days
 * 1 	January   31
 * 2 	February  28 (29 in leap years)
 * 3    March 	  31
 * 4    April     30
 * 5    May       31
 * 6 	June      30
 * 7    July      31
 * 8    August    31
 * 9    September 30
 * 10   October   31
 * 11   November  30
 * 12   December  31
 *
 * @param month month (1-12)
 * @param year  years since 2000
 */
static uint64_t secs_in_month(int month, int year)
{
    uint64_t days = 0;
    const int days_of_month[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    year += 2000;
    if (month == 2 && is_leap_year(year))
        days = 29;
    else
        days = days_of_month[month - 1];
    return days * 86400;
}

/**
 * @brief get unix timestamp from CMOS values
 *
 * @param data cmos data
 */
static uint64_t cmos_get_unix_timestamp(uint16_t *data)
{
    return secs_since_epoch(cmos_get(CMOS_YEAR, data) - 1) +
        secs_in_month(cmos_get(CMOS_MONTH, data) - 1, (cmos_get(CMOS_YEAR, data))) +
        (cmos_get(CMOS_DAY, data) - 1) * 86400 +
        (cmos_get(CMOS_HOUR, data) * 3600) +
        (cmos_get(CMOS_MINUTE, data) * 60) +
        (cmos_get(CMOS_SECOND, data));
}

uint64_t cmos_to_unix(void)
{
    cmos_update();
    do {
        while (is_cmos_updating());
        memcpy(prev_data, data, 128);
        cmos_dump(data);
    } while ((prev_data[CMOS_SECOND] != data[CMOS_SECOND]) ||
        (prev_data[CMOS_MINUTE] != data[CMOS_MINUTE]) ||
        (prev_data[CMOS_HOUR] != data[CMOS_HOUR])     ||
        (prev_data[CMOS_DAY] != data[CMOS_DAY])       ||
        (prev_data[CMOS_MONTH] != data[CMOS_MONTH])   ||
        (prev_data[CMOS_YEAR] != data[CMOS_YEAR]));

    return cmos_get_unix_timestamp(data);
}

struct tm *cmos_time(void)
{
    cmos_update();
    static struct tm time;
    time.tm_sec   = cmos_get(CMOS_SECOND, data);
    time.tm_min   = cmos_get(CMOS_MINUTE, data);
    time.tm_hour  = cmos_get(CMOS_HOUR, data);
    time.tm_mday  = cmos_get(CMOS_DAY, data);
    time.tm_mon   = cmos_get(CMOS_MONTH, data);
    time.tm_year  = cmos_get(CMOS_YEAR, data);
    time.tm_wday  = 0;
    time.tm_yday  = 0;
    time.tm_isdst = 0;
    return &time;
}
