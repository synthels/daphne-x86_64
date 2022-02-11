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
 * HPET (TODO)
 */

#include "hpet.h"

static uint64_t ticks = 0;
static struct tm_func_node *head_fn;

void hpet_init(struct tm_func_node *head)
{
    /* Check if HPET is available */
    head_fn = head;
    if (acpi_get_entry("HPET")) {
        info("time: switched default time source to hpet");
        time_source_set(TIME_SOURCE_HPET);
    }
}

/* HPET IRQ */
void hpet_irq_handler(regs_t *r)
{
    hpet_tick(r);
}

void hpet_tick(regs_t *r)
{
    ++ticks;
    struct tm_func_node *current = head_fn;
    /* Call every hooked function */
    while (current->next != NULL) {
        (current->_this)(r, ticks);
        current = current->next;
    }
}

uint64_t hpet_get_jiffies(void)
{
    return ticks;
}
