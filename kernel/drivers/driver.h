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

#ifndef DRIVER
#define DRIVER

#include <stdint.h>
#include <kernel.h>
#include <tty/printk.h>

#include <drivers/keyboard/keyboard.h>
#include <drivers/sound/pcspk/pcspk.h>
#include <drivers/time/PIT.h>
#include <drivers/pci/pci.h>

#define MODULE_NAME(x) static char *__mod_name __attribute((unused)) = x
#define MODULE_AUTH(x) static char *__mod_auth __attribute((unused)) = x

#define mprint(x) printk("%s: %s", __mod_name, x)

/* Init drivers */
void init_drivers(void);

#endif
