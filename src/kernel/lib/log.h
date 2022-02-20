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

#define log(level, fmt, ...) printk(level, "[  %i.%i] " fmt "\n", time(), uptime(), ##__VA_ARGS__)

#define pr_info(fmt, ...) log(NORMAL, fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...) log(WARNING, fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...) log(ERROR, fmt, ##__VA_ARGS__)
#define pr_test(fmt, ...)log(ERROR, fmt, ##__VA_ARGS__)

