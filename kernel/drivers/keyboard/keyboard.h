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
 */

#ifndef DRIVER_KEYBOARD
#define DRIVER_KEYBOARD

#include <io/io.h>
#include <stdint.h>
#include <drivers/driver.h>
#include <memory/lmmap/lmmap.h>

struct driver_interface *kbd_interface;

#define kbd_send_command(x) outb(0x64, x)

/* Init keyboard */
void kbd_init(void);

/* Get last key */
void kbd_get_last_key(uint8_t *key);

/* Read 0x60 */
void kbd_read();

#endif
