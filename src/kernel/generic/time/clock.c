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
 * Clock functions
 */

#include "clock.h"

static struct tm_func_node node = { NULL, NULL };

void clock_init(void)
{
    /* Boot up te PIT as a first time source... */
    pit_init();
    lapic_irq_enable(0);
}

struct tm_func_node *clock_get_root_func_node(void)
{
    return &node;
}

void clock_hook(tm_func_t f)
{
    struct tm_func_node *current = &node;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = kmalloc(sizeof(struct tm_func_node));
    current->_this = f;
    current->next->next = NULL;
}

uint64_t clock_get_ticks(void)
{
    return time_source_get_jiffies();
}

void clock_run_hooks(regs_t *r, uint64_t jiffies)
{
    /* Call every hooked function */
    struct tm_func_node *current = &node;
    while (current->next != NULL) {
        (current->_this)(r, jiffies);
        current = current->next;
    }
}
