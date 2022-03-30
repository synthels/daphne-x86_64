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
 * APIC timer
 */

#include "apic.h"

void apic_init(void)
{
    lapic_write(TIMER_DIV, 0x3);
    lapic_write(TIC, 0xffffffff);

    millisleep(100);

    lapic_write(LVT_TIMER, 0x10000);

    uint64_t onems = (0xffffffff - lapic_read(TCURR)) / 100;

    lapic_write(LVT_TIMER, 32 | 0x20000);
    lapic_write(TIMER_DIV, 0x3);
    lapic_write(TIC, onems);

    /* If the APIC timer is intialised by an AP,
       setting the time source will cause a GPF */
    if (this_core->is_bsp) {
        time_source_set(TIME_SOURCE_APIC);
    }
}
