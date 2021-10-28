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

static struct pci_device root_device;

void pci_scan_bus(int bus);

uint16_t pci_read_field(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t size)
{
    uint32_t addr = (uint32_t) (bus << 16) | (device << 11) | (function << 8) | (offset & 0xFC) | (1 << 31);
    outl(PCI_ADDRESS_PORT, addr);

    switch (size) {
        case 1: return inb(PCI_VALUE_PORT + (offset & 3)); break;
        case 2: return ind(PCI_VALUE_PORT + (offset & 2)); break;
        case 4: return inl(PCI_VALUE_PORT); break;
        default: return 0x0; break;
    }
}

void write_dword(uint32_t bus, uint32_t device, uint32_t function, uint8_t offset, uint32_t value)
{
    uint32_t target = (1 << 31) | (bus << 16) | ((device & 0b11111) << 11) | ((function & 0b111) << 8) | (offset & ~(0b11));
    outl(PCI_ADDRESS_PORT, target);
    outl(PCI_VALUE_PORT, value);
}

void pci_scan_func(int bus, int slot, int func)
{
    static struct pci_device *last_dev = &root_device;
    uint32_t vend = PCI_GET_VENDOR_ID(slot, bus, func);
    uint32_t class = PCI_GET_CLASS(slot, bus, func);
    uint32_t subclass = PCI_GET_SUBCLASS(slot, bus, func);
    if (vend == PCI_NONE) {
        return;
    }
    info(
        "pci: device located {vendor: 0x%x, class: 0x%x, subclass: 0x%x} @ %ui:%ui:%ui", 
        vend, class, 
        subclass, bus, 
        slot, func
    );
    struct pci_device *dev = kmalloc(sizeof(struct pci_device));
    dev->class_id = class;
    dev->subclass_id = subclass;
    dev->vendor_id = vend;
    dev->bus = bus;
    dev->slot = slot;
    dev->func = func;
    dev->next = NULL;
    last_dev->next = dev;
    last_dev = dev;

    /* Handle bridge */
    if (PCI_GET_CLASS(slot, bus, func) == PCI_CLASS_BRIDGE) {
        pci_scan_bus(pci_read_field(bus, slot, func, PCI_SECONDARY_BUS, 1));
    }
}

static void send_address(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    outd(PCI_ADDRESS_PORT, 0x80000000 | (bus << 16) | (slot << 11) | (function << 8) | (offset & 0xfc));
}

void pci_cfg_write_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t data)
{
    send_address(bus, slot, function, offset);
    outd(PCI_VALUE_PORT + (offset & 2), data);
}

void pci_cfg_write_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data)
{
    send_address(bus, slot, function, offset);
    outd(PCI_VALUE_PORT, data);
}

uint32_t pci_cfg_read_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    send_address(bus, slot, function, offset);
    return ind(PCI_ADDRESS_PORT);
}

void pci_scan_slot(int bus, int slot)
{
    for (int func = 0; func < 8; func++) {
        pci_scan_func(bus, slot, func);
    }
}

void pci_scan_bus(int bus)
{
    for (int slot = 0; slot < 32; ++slot) {
        pci_scan_slot(bus, slot);
    }
}

void pci_scan(void)
{
    for (int bus = 0; bus < 256; ++bus) {
        pci_scan_bus(bus);
    }
}

struct pci_bar_data pci_get_bar(uint64_t bar, struct pci_device *dev)
{
    struct pci_bar_data ret;
    uint32_t value = pci_read_field(dev->bus, dev->slot, dev->func, PCI_CFG_BAR0 + (bar * 4), 4);
    if (value & 1) {
        ret.base = (uint64_t) ((uint16_t) value & ~0x03);
    }
    if (((value >> 1) & 0x03) == 0) {
        ret.base = (uint64_t) (value & ~0x0f);
    }
    if (((value >> 1) & 0x03) == 2) {
        ret.base = ((uint64_t) pci_cfg_read_dword(dev->bus, dev->slot, dev->func, PCI_CFG_BAR0 + ((bar + 1) * 4)) << 32) | (value & ~0x0f);
    }

    return ret;
}

struct pci_device *pci_fetch(uint32_t class, uint32_t subclass)
{
    struct pci_device *dev = &root_device;
    for (;;) {
        if (class == dev->class_id && subclass == dev->subclass_id) return dev;
        if (dev->next == NULL) return NULL;
        dev = dev->next;
    }
}
