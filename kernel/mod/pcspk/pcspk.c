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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR See the
 * GNU General Public License for more details.
 *
 * PC Speaker driver
 */

#include "pcspk.h"

/* Play a certain frequency through the PC speaker */
static void _beep(uint16_t freq)
{
    if (freq == 0) { return; }  

    /* Set frequency */
    uint32_t div = 1193180 / freq;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) div);
    outb(0x42, (uint8_t) (div >> 8));

    /* Play the sound */
    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

/* Stop the speaker */
static void _stop(void)
{
    outb(0x61, inb(0x61) & 0xfc);
}

void pcspk_beep(uint16_t freq, uint16_t ticks)
{
    _beep(freq);
    sleep(ticks);
    _stop();
}
