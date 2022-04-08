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

#include <generic/malloc/malloc.h>
#include <generic/sched/task.h>
#include <generic/fs/vfs.h>

#include <lib/string.h>
#include <lib/elf.h>

/**
 * exec
 *   brief: execute the file located at some path
 */
int exec(const char *path, int argc, const char *argv);
