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

#ifndef LIBK_PRINTK
#define LIBK_PRINTK

#include <stdarg.h>
#include <stdint.h>
#include <shrimp/shrimp.h>
#include <forbia/kernel.h>
#include <forbia/types.h>

#include <mem/malloc.h>
#include <libk/stdlib.h>

#define __PRINTK_BUFFER_SIZE 256

enum printk_log_level {
    NORMAL  = 0,
    WARNING = 1,
    ERROR   = 2,
    PANIC   = 3
};

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

/* Shrimp colors */
#define KERN_COLOR_SUCCESS "\xff[10v0020"
#define KERN_COLOR_INFO "\xff[10vv022"
#define KERN_COLOR_WARN "\xff[1vu0210"
#define KERN_COLOR_ERR "\xff[1v00200"
#define KERN_COLOR_RESET "\xff[1ffffff"

#define ok(fmt, ...)    printk(NORMAL, "%s[  ok  ]%s " fmt "\n", KERN_COLOR_SUCCESS, KERN_COLOR_RESET, ##__VA_ARGS__)
#define info(fmt, ...)  printk(NORMAL, "%s[  info  ]%s " fmt "\n", KERN_COLOR_INFO, KERN_COLOR_RESET, ##__VA_ARGS__)
#define warn(fmt, ...)  printk(NORMAL, "%s[  warning  ]%s " fmt "\n", KERN_COLOR_WARN, KERN_COLOR_RESET, ##__VA_ARGS__)
#define err(fmt, ...)   printk(NORMAL, "%s[  error  ]%s " fmt "\n", KERN_COLOR_ERR, KERN_COLOR_RESET, ##__VA_ARGS__)

#endif
