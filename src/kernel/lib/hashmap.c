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
 *
 * Hashmap
 */

#include "hashmap.h"

static hash_t hash(struct hashmap *map, const char *key)
{
    hash_t hash = 0;
    int c;
    while ((c = *key++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return (hash % map->size);
}

struct hashmap *hashmap(size_t size)
{
    struct hashmap *map = kmalloc(sizeof(struct hashmap));
    map->entries = kmalloc(sizeof(struct hashmap_entry *) * size);
    map->size = size;
    return map;
}

void hashmap_insert(struct hashmap *map, char *key, void *value)
{
    hash_t key_hash = hash(map, key);
    struct hashmap_entry *e = map->entries[key_hash]; 
    if (!e) {
        /* No hash collision, just insert */
        e = kmalloc(sizeof(struct hashmap_entry));
        e->key = key;
        e->value = value;
        map->entries[key_hash] = e;
    } else {
        /* Hash collision, insert as next node
           of duplicate */
        struct hashmap_entry *ptr = NULL;
        do {
            if (e->key == key) {
                e->value = value;
            } else {
                ptr = e;
                e = e->next;
            }
        } while (e);
        e = kmalloc(sizeof(struct hashmap));
        e->key = key;
        e->value = value;
        e->next = NULL;
        ptr->next = e;
    }
}

void *hashmap_get(struct hashmap *map, char *key)
{
    struct hashmap_entry *e = map->entries[hash(map, key)];
    if (!e) {
        return NULL;
    } else {
        /* Iterate over things with the same
           hash until we find the actual thing we
           want */
        do {
            if (e->key == key) return e->value;
            e = e->next;
        } while (e);
        return NULL;
    }
}
