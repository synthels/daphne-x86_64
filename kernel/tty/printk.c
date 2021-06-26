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
 *
 * printk function
 */

#include "printk.h"

/* Don't let anyone see this... */
static char printk_buf[1024];

int printk(const char *fmt, ...)
{
	/* Empty buffer */
	for (size_t i = 0; i < sizeof(printk_buf) / sizeof(char); i++) {
		printk_buf[i] = '\0';
	}

	char *buf;
	/*
	 * Hack to make gcc point to the first parameter 
	 * instead of ignoring it
	 */
	va_list args = (va_list) &fmt;
	int err = vsprintf(&buf, args);
	tty_puts(buf, VGA_COLOR_WHITE);
	va_end(args);

	return err;
}

/*
 * Not too bad, right?
 * FIXME: Using more than one %i can crash the system?
 */
errcode_t vsprintf(char **buf, va_list args)
{
	const char *fmt = va_arg(args, char *);
	char c;
	for (int i = 0; (c = *fmt++);) {
		printk_buf[i++] = c;
		if (c == '%') {
			c = *fmt++;
			char *str;
			switch (c) {
				/* Strings */
				case 's':
					str = va_arg(args, char *);
					break;
				/* Unsigned (no prettier way to do this) */
				case 'u':
					switch (*fmt++) {
						case 'i':
							uitoa(va_arg(args, uint32_t), str);
							break;
						/* Hex */
						case 'x':
							/* TODO */
							break;
						/* Binary */
						case 'b':
							/* TODO */
							break;
					}
					break;
				case 'i':
					itoa(va_arg(args, int32_t), str);
					break;
				/* Hex */
				case 'x':
					/* TODO */
					break;
				/* Binary */
				case 'b':
					/* TODO */
					break;
				/* Just print a '%' */
				case '%':
					str = "%";
					break;
				/* Unknown type */
				default:
					*buf = "";
					return EINVAL;
			}
			/* Skip '%' sign */
			i--;
			/* Copy string to buffer */
			for (; (c = *str++); i++) {
				printk_buf[i] = c;
			}
		}
	}

	*buf = printk_buf;
	return NOERR;
}
