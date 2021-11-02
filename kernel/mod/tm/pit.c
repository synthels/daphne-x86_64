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
 * PIT module
 */

#include "pit.h"

static uint64_t ticks = 0;
static struct tm_func_node *head_fn;

void pit_init(struct tm_func_node *head)
{
    head_fn = head;
    int div = 1193180 / TIMER_FREQ;
    outb(0x43, 0x34);
    outb(0x40, div & 0xFF);
    outb(0x40, div >> 8);
}

void pit_tick(void)
{
    ++ticks;
    struct tm_func_node *current = head_fn;
    /* Call every hooked function */
    while (current->next != NULL) {
        (current->_this)(ticks);
        current = current->next;
    }
}

uint64_t pit_get_ticks(void)
{
    return ticks;
}
