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
 * (dumb) ATA PIO driver
 */

#include "ide.h"

void ide_init(void)
{
    /* Check for disk 1 */
    outb(0x1f6, 0xe0 | (1<<4));
    for(int i = 0; i < 1000; i++){
        if(inb(0x1f7) != 0){
            info("ide: disk 1 present");
            break;
        }
    }

    // Switch back to disk 0.
    outb(0x1f6, 0xe0 | (0<<4));
}
