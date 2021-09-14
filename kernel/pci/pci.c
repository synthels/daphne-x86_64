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
 * PCI functions
 */

#include "pci.h"

uint32_t last_dev = 0x0;

bool pci_scan_bus(int class, int subclass, int bus);

static inline int pci_bus(uint32_t device)
{
    return (uint8_t) (device >> 16);
}

static inline int pci_slot(uint32_t device)
{
    return (uint8_t) (device >> 8);
}

static inline int pci_func(uint32_t device)
{
    return (uint8_t) device;
}

static inline uint32_t pci_get_addr(uint32_t device, int field)
{
    return PCI_ADDR | (pci_bus(device) << 16) | (pci_slot(device) << 11) | (pci_func(device) << 8) | (field & 0xFC);
}

static inline uint32_t pci_pack_dev(int bus, int slot, int func)
{
    return (uint32_t) ((bus << 16) | (slot << 8) | func);
}

uint32_t pci_read_field(uint32_t device, int field, int size)
{
    outl(PCI_ADDRESS_PORT, pci_get_addr(device, field));
    if (size == 4) {
        return inl(PCI_VALUE_PORT);
    } else if (size == 2) {
        return ins(PCI_VALUE_PORT + (field & 2));
    } else if (size == 1) {
        return inb(PCI_VALUE_PORT + (field & 3));
    }

    return PCI_NONE;
}

bool pci_scan_func(int class, int subclass, int bus, int slot, int func) {
    last_dev = pci_pack_dev(bus, slot, func);
    /* Device found */
    if ((class == PCI_GET_CLASS(last_dev)) && (subclass == PCI_GET_SUBCLASS(last_dev))) {
        return true;
    }
    /* Handle bridge */
    if (PCI_GET_CLASS(last_dev) == PCI_CLASS_BRIDGE) {
        pci_scan_bus(class, subclass, pci_read_field(last_dev, PCI_SECONDARY_BUS, 1));
    }
    return false;
}

bool pci_scan_slot(int class, int subclass, int bus, int slot)
{
    uint32_t dev = pci_pack_dev(bus, slot, 0);
    if ((pci_read_field(dev, PCI_VENDOR_ID, 2) == PCI_NONE) || !pci_read_field(dev, PCI_HEADER_TYPE, 1)) {
        return false;
    }
    for (int func = 0; func < 8; func++) {
        uint32_t dev = pci_pack_dev(bus, slot, func);
        if (pci_read_field(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
            if (pci_scan_func(class, subclass, bus, slot, func)) return true;
        }
    }
    return false;
}

bool pci_scan_bus(int class, int subclass, int bus)
{
    for (int slot = 0; slot < 32; ++slot) {
        if (pci_scan_slot(class, subclass, bus, slot)) return true;
    }
    return false;
}

/* TODO: brute force... */
void pci_search(pci_callback_t fn, int class, int subclass)
{
    for (int bus = 0; bus < 256; ++bus) {
        if (pci_scan_bus(class, subclass, bus)) fn(last_dev);
    }
}
