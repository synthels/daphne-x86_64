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

#include "kernel.h"

/* Current kernel mode */
static int kernel_mode;

void set_kernel_mode(int mode)
{
    kernel_mode = mode;
}

int get_kernel_mode(void)
{
    return kernel_mode;
}

void kernel_hang(void)
{
    asm("cli");
    asm("hlt");
}
