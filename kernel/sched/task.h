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

#include <memory/context.h>

#define STASH_SIZE 64

typedef int16_t pid_t;

enum task_state {
    ACTIVE = 0,
    SLEEPING = 1,
    DEAD = 2
};

struct context_stash {
    context_t **contexts;
    uint64_t size;
};

struct task {
    pid_t pid;
    pid_t parent_pid;
    int state;
    context_t *context;
    char *name;
    struct task *next;
};

/**
 * get_head_task
 *   brief: get head task
 */
struct task *get_head_task(void);

/**
 * init_stash
 *   brief: init pt stash
 */
void init_stash(void);

/**
 * get_stashed_context
 *   brief: get context from stash
 */
context_t *get_stashed_context(void);

/**
 * stash_page_table
 *   brief: add context to stash
 */
void stash_context(context_t *context);

/**
 * ktask_run
 *   brief: create new task
 */
pid_t ktask_run(char *name);
