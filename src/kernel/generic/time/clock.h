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
#include <modules/pit/pit.h>

#include <generic/sched/task.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/cmos.h>
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/irq.h>
#endif

#include "jiffies.h"
#include "time_func_node.h"
#include "time_source.h"

/**
 * clock_init
 *   brief: init tm module
 */
void clock_init(void);

/**
 * clock_hook
 *   brief: hook f to the timer interrupt
 */
void clock_hook(tm_func_t f);

/**
 * clock_get_jiffies
 *   brief: get current jiffy count
 */
uint64_t clock_get_jiffies(void);

/**
 * clock_get_root_func_node
 *   brief: get root func node
 */
struct tm_func_node *clock_get_root_func_node(void);

/**
 * clock_run_hooks
 *   brief: run all hooked functions
 */
void clock_run_hooks(regs_t *r, uint64_t jiffies);
