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

#ifndef KERNEL_x86_TTY
#define KERNEL_x86_TTY

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.h"

/* Initialise the terminal */
void tty_init();

/* Clears the terminal & fills it with a certain color */
void tty_clear(uint8_t color);

/* Print single character */
void tty_putc(char c, unsigned char color);

/* Print string */
void tty_print(const char *data, const unsigned char color);

/* Print string & append newline */
void tty_puts(const char *data, const unsigned char color);

#endif
