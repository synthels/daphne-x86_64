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

/* Init keyboard */
void kbd_init(void);

/* Get last key */
void kbd_get_last_key(uint8_t *key, int *pressed);

/* Read 0x60 */
void kbd_read();

/* Check if keyboard is enabled */
int kbd_is_enabled(void);

/* Enable/disable keyboard */
void kbd_set_enabled(int enabled);

/* Check if there is a key event */
int kbd_get_event();

/* Acknowledge that keyboard event was read */
void kbd_ack();

/* Translates a key corrensponding to a specific layout */
uint8_t kbd_translate(uint8_t key, uint8_t layout[]);

/* Check if a key is a printable character */
int kbd_is_printable(uint8_t key, uint8_t layout[]);

#endif
