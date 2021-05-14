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
 *
 * TTY functions
 */

#include "tty_io.h"

/* TODO: Move this to lib */
static size_t strlen(const char *str)
{
	size_t i = 0;
	while (*str++) { i++; };
	return i;
}

/* Pack color & character in a VGA entry */
static inline uint16_t pack_vga(unsigned char c, uint8_t color)
{
	return (uint16_t) c | (uint16_t) color << 8;
}

/* Put character c at cell (x, y) */
static void tty_put_at(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	tty_buffer[index] = pack_vga(c, color);
}

/* Clears the terminal & fills it with a certain color */
void tty_clear(uint8_t color)
{
	tty_column = 0;
	tty_row = 0;
	tty_color = VGA_COLOR_WHITE | color << 4;
	tty_bg = color;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			tty_buffer[index] = pack_vga(' ', tty_color);
		}
	}
}

/* Initialise the terminal */
void tty_init()
{
	/* Initialise terminal */
	tty_row = 0;
	tty_column = 0;
	/* Point to video memory */
	tty_buffer = (uint16_t *) VID_MEM;

	/* Set background color */
	tty_clear(VGA_COLOR_BLACK);
}

/* Print single character */
void tty_putc(char c, unsigned char color)
{
	tty_color = color | tty_bg << 4;

	/* We don't really need tabs */
	if (c == '\t') return;

	/* Change row for newlines */
	if (c == '\n') {
		tty_column = 0;
		/* Scroll terminal if it fills up */
		if (++tty_row >= VGA_HEIGHT) {
			tty_row = VGA_HEIGHT-1;
			for (size_t i = 0; i < VGA_HEIGHT; i++) {
				for (size_t j = 0; j < VGA_WIDTH; j++) {
					/* Move all lines up one row */
					tty_buffer[i * VGA_WIDTH + j] = tty_buffer[(i + 1) * VGA_WIDTH + j];
				}
			}
		}

		return;
	}

	/* Print character */
	tty_put_at(c, tty_color, tty_column, tty_row);
	if (++tty_column == VGA_WIDTH) {
		tty_column = 0;
	}
}

/* Print string */
void tty_print(const char *data, const unsigned char color)
{
	const size_t size = strlen(data);
	size_t k = 0;
	for (size_t i = 0; i < size; i++) {
		/* Skip spaces on newline */
		if (data[i] == ' ') {
			if (data[i-1] == '\n') {
				continue;
			}
		}
		
		/* Go to next line when this line is filled up */
		if (k >= VGA_WIDTH && data[i] != '\n') {
			tty_putc('\n', color);
			k = 0;
		}
		tty_putc(data[i], color);
		k++;
	}
}

/* Print string & append newline */
void tty_puts(const char *data, const unsigned char color)
{
	tty_print(data, color);
	/* Insert newline */
	tty_putc('\n', color);
}
