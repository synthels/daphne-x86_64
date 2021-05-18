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
 * Sleep function
 */

#include "sleep.h"

/* Sleep n ticks... */
void sleep(uint32_t ticks)
{
	uint32_t start;
	uint32_t now;
	pit_get_ticks(&start);
	pit_get_ticks(&now);
	while ((now - start) < ticks) {
		pit_get_ticks(&now);
	}
}
