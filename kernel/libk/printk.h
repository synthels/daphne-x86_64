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

/* Prints a formatted string to the screen using tty functions */
int printk(const char *fmt, ...);

#endif
