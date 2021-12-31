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

#pragma once

#include <generic/io/io.h>
#include <generic/forbia/kernel.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
#endif

#include "tm.h"

#define TIMER_FREQ 100

/**
 * pit_init
 *   brief: init pit
 */
void pit_init(struct tm_func_node *head);

/**
 * pit_tick
 *   brief: do one tick
 */
void pit_tick(regs_t *r);

/**
 * pit_get_ticks
 *   brief: get current tick count
 */
uint64_t pit_get_ticks(void);
