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
 * Time module
 */

#include "pit.h" /* TODO: APIC... */
#include "tm.h"

static struct tm_func_node node = { NULL, NULL };

void tm_init(void)
{
    pit_init(&node);
}

void tm_hook(tm_func_t f)
{
    struct tm_func_node *current = &node;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = kmalloc(sizeof(struct tm_func_node));
    current->_this = f;
    current->next->next = NULL;
}

uint64_t tm_get_ticks(void)
{
    return pit_get_ticks();
}
