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
 * Serial port debugging
 */

#include "debug.h"

/**
 * @brief Check if DEBUG_SERIAL_PORT is empty
 */
static int serial_is_empty(void)
{
    return inb(DEBUG_SERIAL_PORT + 5) & 0x20;
}

/**
 * @brief Write single character to DEBUG_SERIAL_PORT
 */
static void serial_write(char out)
{
    while (!serial_is_empty());
    outb(DEBUG_SERIAL_PORT, out);
}

/**
 * @brief Write whole string to DEBUG_SERIAL_PORT
 */
void debug(const char *out)
{
    for (uint32_t i = 0; i < strlen(out); ++i) {
        serial_write(out[i]);
    }
}
