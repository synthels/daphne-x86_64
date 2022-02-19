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

#include <lib/printk.h>

#include <mod/pit/pit.h>
#include <mod/apic/apic.h>

enum time_source {
    TIME_SOURCE_PIT,
    TIME_SOURCE_APIC,
    TIME_SOURCE_HPET,
    TIME_SOURCE_NONE
};

/**
 * time_source_irq_handler
 *   brief: time irq handler
 */
void time_source_irq_handler(regs_t *r);

/**
 * time_source_set
 *   brief: set current best time source
 */
void time_source_set(enum time_source t);

/**
 * time_source_get_jiffies
 *   brief: get current jiffy count
 */
int time_source_get_jiffies(void);
