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
 * Doubly linked list
 */

#include "list.h"

struct list *list(void)
{
    struct list *ls = kmalloc(sizeof(struct list));
    ls->head = NULL;
    ls->tail = ls->head;
    ls->size = 0;
    return ls;
}

struct list_node *list_insert(struct list *ls, void *data)
{
    struct list_node *node = kmalloc(sizeof(struct list_node));
    node->data = data;
    node->next = NULL;
    if (ls->size < 1) {
        ls->head = node;
        ls->tail = node;
        node->prev = NULL;
        ls->size++;
        return node;
    }
    ls->tail->next = node;
    node->prev = ls->tail;
    ls->tail = node;
    ls->size++;
    return node;
}

void list_insert_multiple(struct list *ls, void *data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        list_insert(ls, data);
    }
}

void list_insert_after(struct list *ls, struct list_node *prev, void *data)
{
    if (!prev) {
        return;
    }

    struct list_node *new = kmalloc(sizeof(struct list_node));
    new->data = data;

    new->next = prev->next;
    prev->next = new;
    prev->prev = prev;
    ls->size++;

    if (new->next) {
        new->next->prev = new;
    }
}

void list_remove(struct list *ls, struct list_node *node)
{
    struct list_node *prev;
    if (!node) {
        return;
    } else {
        while (node->next) {
            node->data = node->next->data;
            prev = node;
            node = node->next;
        }
        prev->next = NULL;
    }
    ls->size--;
}

struct list_node *list_get(struct list *ls, size_t i)
{
    struct list_node *it = ls->head;
    if (i < ls->size) {
        for (size_t j = 0; j < i; ++j) {
            it = it->next;
        }
        return it;
    }
    return NULL;
}

void list_merge(struct list *l1, struct list *l2)
{
    l1->tail->next = l2->head;
    l1->size += l2->size;
}

void list_destroy(struct list *ls)
{
    list_foreach(ls, i, _i) {
        kfree(i);
    }
    kfree(ls);
}
