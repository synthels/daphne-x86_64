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

#include <stdarg.h>
#include <stdint.h>

#include <generic/shrimp/shrimp.h>

#include <generic/forbia/kernel.h>
#include <generic/forbia/types.h>
#include <lib/lock.h>
#include <generic/malloc/malloc.h>

#include <lib/stdlib.h>
#include <lib/string.h>

#define VSPRINTF_BUFFER_SIZE 512

enum printk_log_level {
    NORMAL   = 0,
    WARNING  = 1,
    ERROR    = 2,
    TEST     = 3,
    PANIC    = 4,
};

typedef int (*printk_stream)(const char *);

/**
 * printk
 *   brief: print formatted string to kernel terminal
 *   parameters:
 *     - level:  log level
 */
int printk(int level, const char *fmt, ...);

/**
 * get_log_level
 *   brief: get printk log level
 */
int get_log_level(void);

/**
 * set_log_level
 *   brief: set printk log level
 */
void set_log_level(int level);

/**
 * vsprintf
 *   brief: print formatted string to buffer
 */
int vsprintf(char *buf, const char *fmt, va_list args);

/**
 * vfprintf
 *   brief: print formatted string to stream
 */
int vfprintf(printk_stream stream, const char *fmt, va_list args);
