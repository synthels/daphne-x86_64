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

#ifndef DRIVER_SPEAKER
#define DRIVER_SPEAKER

#include <stdint.h>
#include <io/io.h>
#include <drivers/driver.h>
#include <drivers/time/sleep.h>

/*  
 * Play a certain frequency through the PC speaker 
 * for a certain amount of ticks
*/
void pcspk_beep(uint16_t freq, uint16_t ticks);

#endif
