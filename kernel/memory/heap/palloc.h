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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef PALLOC
#define PALLOC

#include <stdint.h>
#include <stddef.h>
#include <logger/panic.h>
#include <memory/init_module/init_module.h>

/* Init palloc */
void __mm_init_palloc__(void);

/* Allocates n bytes */
/* Only used by mm, not the rest of the kernel */
uint32_t *palloc(size_t n);

#endif
