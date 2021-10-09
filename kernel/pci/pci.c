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

void pci_scan_bus(int class, int subclass, int bus, pci_callback_t fn);

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
    return (uint32_t) ((bus << 16) | (slot << 8) | func << 5);
}

uint16_t pci_read_field(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
	uint32_t addr = (uint32_t)(bus << 16) | (device << 11) | (function << 8) | (offset & 0xFC) | (1 << 31);
	outb(PCI_ADDRESS_PORT, addr);
	uint16_t tmp = (uint16_t)((inb(PCI_VALUE_PORT) >> ((offset & 2) * 8)) & 0xFFFF);
	return tmp;
}

void pci_scan_func(int class, int subclass, int bus, int slot, int func, pci_callback_t fn) {
    uint32_t dev = pci_pack_dev(bus, slot, func);
    /* Device found */
    if ((class == PCI_GET_CLASS(dev, bus, func)) && (subclass == PCI_GET_SUBCLASS(dev, bus, func))) {
        fn(dev);
    }
    /* Handle bridge */
    if (PCI_GET_CLASS(dev, bus, func) == PCI_CLASS_BRIDGE) {
        pci_scan_bus(class, subclass, pci_read_field(bus, dev, func, PCI_SECONDARY_BUS), fn);
    }
}

void pci_scan_slot(int class, int subclass, int bus, int slot, pci_callback_t fn)
{
    for (int func = 0; func < 8; func++) {
        uint32_t dev = pci_pack_dev(bus, slot, func);
        if (pci_read_field(bus, dev, func, PCI_VENDOR_ID) != PCI_NONE) {
            pci_scan_func(class, subclass, bus, slot, func, fn);
        }
    }
}

void pci_scan_bus(int class, int subclass, int bus, pci_callback_t fn)
{
    for (int slot = 0; slot < 32; ++slot) {
        pci_scan_slot(class, subclass, bus, slot, fn);
    }
}

/* TODO: brute force... Obviously we need to do this once &
         then pick from an array */
void pci_search(pci_callback_t fn, int class, int subclass)
{
    for (int bus = 0; bus < 256; ++bus) {
        pci_scan_bus(class, subclass, bus, fn);
    }
}
