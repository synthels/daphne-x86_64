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

#include <generic/fs/vfs.h>
#include <generic/malloc/malloc.h>
#include <generic/memory/mem.h>
#include <generic/sched/task.h>
#include <generic/forbia/errno.h>
#include <generic/forbia/kernel.h>

#include <lib/printk.h>
#include <lib/string.h>
#include <lib/lock.h>
#include <lib/elf.h>

#include <stdint.h>

/* Describes an executable
   object */
struct binfmt_object {
    char *name;
    void *buf;
};

/**
 * binfmt_init
 *   brief: Init binfmt
 */
void binfmt_init(void);

/**
 * exec
 *   brief: Schedule executable located at some path
 *          to run
 *
 *   parameters:
 *     - path: path to executable
 *     - argv: arguments
 *     - envp: environment variables
 */
int exec(const char *path, const char *argv[], const char *envp[]);
