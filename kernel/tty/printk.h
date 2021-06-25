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

#ifndef PRINTK
#define PRINTK

#include <stdarg.h>
#include <stdint.h>
#include <kernel.h>

#ifdef ARCH_x86_32
#include <arch/x86_32/tty_io.h>
#else
#error "x64 does not support tty yet!"
#endif

#include <libk/stdlib.h>

/* Prints a formatted string to a buffer */
int vsprintf(char **buf, va_list args);

/* Prints a formatted string to the screen using tty functions */
int printk(const char *fmt, ...);

#endif
