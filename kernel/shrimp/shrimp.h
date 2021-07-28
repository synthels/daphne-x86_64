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

#ifndef __FBTERM
#define __FBTERM

#define FBTERM_OFFSET 5

#include <forbia/kernel.h>
#include <vid/lfb.h>
#include <mem/malloc.h>
#include <forbia/types.h>

#include <libk/string.h>
#include <libk/printk.h>

#include "shrimp_font.h"

#define BG_COLOR {12, 12, 12, 255}
#define FG_COLOR {255, 255, 255, 255}

/**
 * shrimp_init
 *   brief: init fbterm
 *   parameters:
 *     - _handle: fbterm context handle
 */
void shrimp_init(int _handle);

/**
 * shrimp_print
 *   brief: print string
 *   parameters:
 *     - str: string
 */
void shrimp_print(char *str);

/**
 * shrimp_kill
 *   brief: kill fbterm
 */
void shrimp_kill(void);

#endif
