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
 *
 * Memory bitmap
 */

#include "bitmap.h"

void new_bitmap(struct bitmap *bmp, size_t size)
{
    bmp->buf = kmalloc(size);
    memset(bmp->buf, 0xff, size / 8);
    bmp->size = size;
}

void bitmap_set(struct bitmap *bmp, size_t i, bool value)
{
    size_t byte = i / 8;
    size_t bit = i % 8;
    if (value)
        bmp->buf[byte] |= (1 << bit);
    else
        bmp->buf[byte] &= ~(1 << bit);
}

bool bitmap_get(struct bitmap *bmp, size_t i)
{
    size_t byte = i / 8;
    size_t bit = i % 8;
    return (bmp->buf[byte] & (1 << bit));
}

int bitmap_find_free(struct bitmap *bmp)
{
    for (size_t i = 0; i < bmp->size; i++) {
        if (bitmap_get(bmp, i)) {
            return i;
        }
    }
    return -1;
}
