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
 * Kernel main code
 */

#ifndef KERNEL_INIT
#define KERNEL_INIT

#include <multiboot.h>

#include <tty/tty_io.h>
#include <io/io.h>
#include <idt/idt.h>
#include <kernel.h>

#include <modules/time/sleep.h>
#include <memory/mm.h>
#include <memory/paging/paging.h>
#include <memory/heap/kmalloc.h>

#include <tty/printk.h>
#include <logger/panic.h>

#endif
