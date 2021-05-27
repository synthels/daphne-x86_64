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

#ifndef LIBK_STRING
#define LIBK_STRING

#include <stddef.h>

#ifndef NULL
	#define NULL (void *) 0x0
#endif

int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dst, const void *src, size_t n);

#endif
