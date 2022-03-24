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

#include <stddef.h>
#include <stdint.h>

#include <generic/malloc/malloc.h>

typedef unsigned int hash_t;

void hash_test(void);

struct hashmap_entry {
    char *key;
    void *value;
    struct hashmap_entry *next;
};

struct hashmap {
    size_t size;
    struct hashmap_entry **entries;
};

/**
 * hashmap
 *   brief: allocate new hashmap
 */
struct hashmap *hashmap(size_t size);

/**
 * hashmap_insert
 *   brief: insert key value pair to hashmap
 */
void hashmap_insert(struct hashmap *map, char *key, void *value);

/**
 * hashmap_get
 *   brief: get hashmap element
 */
void *hashmap_get(struct hashmap *map, char *key);
