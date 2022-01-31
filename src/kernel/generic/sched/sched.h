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

#include <generic/sched/task.h>
#include <stddef.h>
#include <mod/tm/tm.h>
#include <generic/forbia/kernel.h>
#include <generic/forbia/lock.h>
#include <lib/printk.h>
#include <lib/sleep.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/cpu.h>
#endif

/**
 * sched_init
 *   brief: init scheduling
 *   warning: CALL AFTER tm_init!
 */
void sched_init(void);

/**
 * task_run
 *   brief: run task
 */
void task_run(struct task *task);

/**
 * task_yield
 *   brief: yield from a task
 */
void task_yield(struct task *task);

/**
 * task_kill
 *   brief: kill a task
 */
void task_kill(struct task *task);

/**
 * switch_task
 *   brief: switch tasks
 */
void switch_task(struct task *tasks);
