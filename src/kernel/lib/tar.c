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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR See the
 * GNU General Public License for more details.
 *
 * Simple archive parser
 */

#include "tar.h"

static size_t read_size(const char *str)
{
    size_t size = 0;
    size_t count = 1;
    for (size_t j = 11; j > 0; --j, count *= 8)
        size += ((str[j - 1] - '0') * count);
    return size;
}

bool tar_read(void *tar, struct tar_block *block, size_t i)
{
    struct tar_header *header = (struct tar_header *) tar;
    for (size_t j = 0; j < i; j++) {
        if (!header->name[0]) {
            return false;
        }
        size_t size = read_size(header->size);
        header = (struct tar_header *)((char *) header + ((size / 512) + 1) * 512);
        if (size % 512) {
            header = (struct tar_header *)((char *) header + 512);
        }
    }

    if (!header->name[0]) {
        return false;
    }

    /* Copy to block */
    strncpy(block->name, header->name, 100);
    block->size = read_size(header->size);
    block->data = (char *) header + 512;

    return true;
}
