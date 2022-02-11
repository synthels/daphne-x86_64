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

#include <generic/time/time.h>
#include <generic/time/time_source.h>
#include <generic/time/time_func_node.h>
#include <generic/time/jiffies.h>

#include <lib/printk.h>

#include <arch/x86_64/acpi.h> /* TODO: not x64 specific... */

/**
 * hpet_init
 *   brief: init hpet
 */
void hpet_init(struct tm_func_node *head);

/**
 * hpet_tick
 *   brief: do one tick
 */
void hpet_tick(regs_t *r);

/**
 * hpet_irq_handler
 *   brief: HPET IRQ handler
 */
void pit_irq_handler(regs_t *r);

/**
 * hpet_get_jiffies
 *   brief: get jiffies since boot
 */
uint64_t hpet_get_jiffies(void);
