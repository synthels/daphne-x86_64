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

#include <generic/forbia/types.h>

#include <lib/string.h>

#include <generic/malloc/malloc.h>

struct bitmap {
    char *buf;
    size_t size;
};

/**
 * new_bitmap
 *   brief: create new bitmap
 *   parameters:
 *     - bmp: bitmap
 *     - size: bitmap size
 */
void new_bitmap(struct bitmap *bmp, void *data, size_t size);

/**
 * bitmap_set
 *   brief: set/clear bit i in bitmap
 */
void bitmap_set(struct bitmap *bmp, size_t i, bool value);

/**
 * bitmap_get
 *   brief: get bit i in bitmap
 */
bool bitmap_get(struct bitmap *bmp, size_t i);

/**
 * bitmap_find_free
 *   brief: get first free page
 */
int bitmap_find_free(struct bitmap *bmp);
