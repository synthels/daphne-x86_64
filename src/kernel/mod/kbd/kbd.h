/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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

#pragma once

#include <generic/io/io.h>
#include <stdint.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/idt/idt.h>
#endif

/**
 * kbd_init
 *   brief: init keyboard driver
 */
void kbd_init(void);

/**
 * kbd_get_last_key
 *   brief: get latest key event
 *   parameters:
 *     - key: pointer to scan code
 *     - pressed: set if key was pressed,
 *                0 if released
 */
void kbd_get_last_key(uint8_t *key, int *pressed);

/**
 * kbd_read
 *   brief: read from PS/2 port
 */
void kbd_read();

/**
 * kbd_is_enabled
 *   brief: check if keyboard is enabled
 */
int kbd_is_enabled(void);

/**
 * kbd_set_enabled
 *   brief: enable/disable keyboard
 */
void kbd_set_enabled(int enabled);

/**
 * kbd_get_event
 *   brief: check if there is a keyboard event
 */
int kbd_get_event();

/**
 * kbd_ack
 *   brief: flush latest keyboard event
 */
void kbd_ack();
