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
#include <mod/pit/pit.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/irq.h>
#endif

#include "jiffies.h"
#include "time_func_node.h"

/**
 * time_init
 *   brief: init tm module
 */
void time_init(void);

/**
 * time_hook
 *   brief: hook f to timer interrupt
 */
void time_hook(tm_func_t f);

/**
 * time_get_ticks
 *   brief: get ticks as of time of calling
 */
uint64_t time_get_ticks(void);

/**
 * time_get_root_func_node
 *   brief: get root func node
 */
struct tm_func_node *time_get_root_func_node(void);

/**
 * time_run_hooks
 *   brief: run all hooked functions
 */
void time_run_hooks(regs_t *r, uint64_t jiffies);
