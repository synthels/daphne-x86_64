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
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <generic/malloc/malloc.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
#endif

typedef void (*tm_func_t)(regs_t *, uint64_t);

struct tm_func_node {
    tm_func_t _this;
    struct tm_func_node *next;
};

/**
 * tm_init
 *   brief: init tm module
 */
void tm_init(void);

/**
 * tm_hook
 *   brief: hook f to timer interrupt
 */
void tm_hook(tm_func_t f);

/**
 * tm_hook
 *   brief: get ticks as of time of calling
 */
uint64_t tm_get_ticks(void);
