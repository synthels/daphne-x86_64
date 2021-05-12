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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PRINTK
#define PRINTK

#include <stdarg.h>
#include <stdint.h>
#include <tty/tty_io.h>
#include <kernel.h>

/* Prints a formatted string to a buffer */
int vsprintf(char **buf, va_list args);

/* Prints a formatted string to the screen using tty functions */
int printk(const char *fmt, ...);

#endif
