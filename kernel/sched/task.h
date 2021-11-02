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

#include <lib/printk.h>
#include <lib/string.h>

#include <malloc/malloc.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/pmm.h>
    #include <arch/x86_64/vmm.h>
#endif

#include <memory/space.h>

typedef int16_t pid_t;

enum task_state {
    ACTIVE = 0,
    SLEEPING = 1,
    DEAD = 2
};

struct task_cpu_state {
    uint64_t *page_table;
    regs_t regs;
};

struct task {
    pid_t pid;
    pid_t parent_pid;
    int state;
    struct task_cpu_state cpu_state;
    char *name;
    struct task *next;
};

/**
 * get_head_task
 *   brief: get head task
 */
struct task *get_head_task(void);

/**
 * spawn_task
 *   brief: create new process
 */
pid_t spawn_task(char *name);
