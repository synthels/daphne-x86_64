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

#include <generic/time/time.h>

#include "printk.h"

#define log(level, fmt, ...) printk(level, "%s[  %i.%i] " fmt "\n", "\xff[1d3d3d3", time(), uptime(), ##__VA_ARGS__)

#define pr_info(fmt, ...) log(NORMAL, "%sINFO %s" fmt, "\xff[101ff70", "\xff[1d3d3d3", ##__VA_ARGS__)
#define pr_warn(fmt, ...) log(WARNING, "%sWARN %s" fmt, "\xff[1ffc107", "\xff[1d3d3d3", ##__VA_ARGS__)
#define pr_err(fmt, ...)  log(ERROR, "%sERROR %s" fmt, "\xff[1ff4136", "\xff[1d3d3d3", ##__VA_ARGS__)
#define pr_test(fmt, ...) log(TEST, "%sTEST %s" fmt, "\xff[1007bff", "\xff[1d3d3d3", ##__VA_ARGS__)
