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
 * string.h functions
 */

#include "string.h"

size_t strlen(const char *str)
{
	const char *s;
	for (s = str; *s; ++s) { }
	return(s - str);
}

void *memset(void *s, int c, size_t sz)
{
	uint32_t* p;
	uint32_t x = c & 0xff;
	byte xx = c & 0xff;
	byte *pp = (byte *) s;
	size_t tail;

	while (((unsigned int) pp & 3) && sz--)
		*pp++ = xx;
	p = (uint32_t *) pp;

	tail = sz & 3;

	x |= x << 8;
	x |= x << 16;

	sz >>= 2;

	while (sz--)
		*p++ = x;

	pp = (byte *) p;
	while (tail--)
		*pp++ = xx;

	return s;
}
