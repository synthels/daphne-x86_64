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

#include <generic/time/time_source.h>
#include <stdint.h>

/**
 * delay
 *   brief: kernel-wide sleep (jiffies)
 */
void delay(uint64_t jiff);

/**
 * sleep
 *   brief: kernel-wide sleep (sec)
 */
void sleep(int sec);

/* WARNING: millisleep & nanosleep depend on the HZ constant being of a certain value & might not work. */

/**
 * millisleep
 *   brief: kernel-wide sleep (millisec)
 */
void millisleep(int msec);

/**
 * nanosleep
 *   brief: kernel-wide sleep (nanosec)
 */
void nanosleep(int usec);
