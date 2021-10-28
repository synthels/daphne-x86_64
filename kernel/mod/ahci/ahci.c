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
 * AHCI
 */

#include "ahci.h"

void ahci_init(void)
{
    struct pci_device *dev;
    if ((dev = pci_fetch(0x01, 0x06)) != NULL) {
        info("ahci: AHCI controller detected (%ui:%ui:%ui)", dev->bus, dev->slot, dev->func);
    } else {
        warn("ahci: No AHCI controller found!");
        return;
    }
    struct pci_bar_data bar = pci_get_bar(5, dev);
    info("ahci: bar base: 0x%x", bar.base);
}
