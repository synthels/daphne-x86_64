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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * Kernel panic
 */

#include "panic.h"

void panic(const char *msg)
{
    printk(PANIC, "kernel panic: %s (on CPU %u)\n", msg, this_core->cpu_id);
    debug(msg); /* Yes, I really couldn't live without this */
    kernel_hang();
}
