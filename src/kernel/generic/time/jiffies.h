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

#ifdef ARCH_x86_64
    #define HZ 100 /* TODO: this should be a kernel parameter */
#endif

int jiffies_to_sec(int jiff);
int jiffies_to_msec(int jiff);
int jiffies_to_usec(int jiff);
int sec_to_jiffies(int sec);
int msec_to_jiffies(int millis);
int usec_to_jiffies(int millis);
