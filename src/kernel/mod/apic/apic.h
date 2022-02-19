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

#include <generic/time/clock.h>
#include <generic/time/time_source.h>
#include <generic/time/time_func_node.h>
#include <generic/time/jiffies.h>
#include <generic/time/sleep.h>

#include <lib/printk.h>

#include <arch/x86_64/acpi.h> /* TODO: not x64 specific... */
#include <arch/x86_64/tsc.h>
#include <arch/x86_64/smp.h>

/**
 * apic_init
 *   brief: init apic timer
 */
void apic_init(void);
