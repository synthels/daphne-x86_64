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
 * TSC
 */

#include "tsc.h"

#include <lib/log.h>

static uint64_t tsc_mhz;

void tsc_init(void)
{
    /**
     * Calibrate TSC using current timer
     */
    int start = rdtsc();
    delay(HZ);
    int end = rdtsc();
    tsc_mhz = (end - start) / 1000000;
}

uint64_t rdtsc(void)
{
    uint32_t low, hi;
    asm volatile ( "rdtsc" : "=a"(low), "=d"(hi) );
    return ((uint64_t) hi << 32UL) | (uint64_t) low;
}

uint64_t tsc_get_mhz(void)
{
    return tsc_mhz;
}

void tsc_delay(uint64_t amount)
{
    uint64_t clock = rdtsc();
    while (rdtsc() < clock + amount * tsc_mhz);
}
