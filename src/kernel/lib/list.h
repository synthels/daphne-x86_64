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

struct list_node {
    void *data;
    struct list_node *next;
    struct list_node *prev;
};

struct list {
    struct list_node *head;
    struct list_node *tail;
    size_t size;
};

#define list_foreach(ls, it, idx) \
    struct list_node *it = ((struct list *) ls)->head; \
    size_t idx = 0; \
    for (; idx < ls->size; it = it->next, idx++)

/**
 * list
 *   brief: allocate new list
 */
struct list *list(void);

/**
 * list_insert
 *   brief: append node to tail of list
 */
void list_insert(struct list *ls, void *data);

/**
 * list_insert_multiple
 *   brief: append same node multiple times
 */
void list_insert_multiple(struct list *ls, void *data, size_t size);

/**
 * list_insert_after
 *   brief: append node next to prev
 */
void list_insert_after(struct list *ls, struct list_node *prev, void *data);

/**
 * list_remove
 *   brief: remove node from list
 */
void list_remove(struct list_node *node);

/**
 * list_get
 *   brief: get element of list
 */
struct list_node *list_get(struct list *ls, size_t i);

/**
 * list_merge
 *   brief: merge l2 into l1
 */
void list_merge(struct list *l1, struct list *l2);

/**
 * list_destroy
 *   brief: free list
 */
void list_destroy(struct list *ls);
