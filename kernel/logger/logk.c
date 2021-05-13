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
 *
 * Logger
 */

#include "logk.h"

void logk(char *msg)
{
    uint32_t ticks;
    pit_interface->read(&ticks);
    switch (kernel_mode) {
        case KERNEL_MODE:
            printk("[%is] %s", ticks, msg);
            break;
        case OS_MODE:
            /* Redirect to a log file once we have an fs... */
            break;
    }
}
