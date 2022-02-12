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
 * jiffies
 *
 * A "jiffie" (as adapted from linux) is defined as 1s * 1/HZ &
 * is the smallest time unit the kernel can perceive
 */

#include "jiffies.h"

int jiffies_to_sec(int jiff)
{
    return jiff / HZ;
}

int jiffies_to_msec(int jiff)
{
    return (jiff / HZ) * 1000;
}

int jiffies_to_usec(int jiff)
{
    return (jiff / HZ) * 1000000;
}

int sec_to_jiffies(int sec)
{
    return sec * HZ;
}

int msec_to_jiffies(int millis)
{
    if (HZ >= 1000)
        return (millis * HZ) / 1000;
    return -1;
}

int usec_to_jiffies(int millis)
{
    if (HZ >= 1000000)
        return (millis * HZ) / 1000000;
    return -1;
}
