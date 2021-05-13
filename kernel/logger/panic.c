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
 * Kernel panic
 */


#include "panic.h"

/* Print a border */
void print_border(void)
{
	for (int i = 0; i < 30; i++) {
		tty_putc('!', VGA_COLOR_LIGHT_RED);
	}
	printk("");
}

/* Print a seperator */
void print_sep(void)
{
	for (int i = 0; i < 30; i++) {
		tty_putc('-', VGA_COLOR_LIGHT_RED);
	}
	printk("");
}

void panic(char *msg)
{
	tty_clear(VGA_COLOR_BLACK);
	print_border();
	tty_puts("// What? It's not my fault!", VGA_COLOR_LIGHT_GRAY);
	print_sep();
	printk("kernel panic: %s", msg);
	print_border();

	/* TODO: Add debug info here */
	for(;;);
}
