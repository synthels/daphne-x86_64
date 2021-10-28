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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR See the
 * GNU General Public License for more details.
 */

#ifndef DEV_PCSPK
#define DEV_PCSPK

#include <stdint.h>
#include <io.h>
#include <libk/sleep.h>

/**
 * pcspk_beep
 *   brief: play freq through the pc speaker for
 *          a few ticks
 */
void pcspk_beep(uint16_t freq, uint16_t ticks);

#endif
