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
 */

#ifndef KERNEL_PCI
#define KERNEL_PCI

#include <stdint.h>
#include <io.h>

#include <forbia/types.h>
#include <libk/printk.h>
#include <malloc.h>

#define PCI_SECONDARY_BUS   0x19
#define PCI_CLASS_BRIDGE    0x06

#define PCI_ADDRESS_PORT    0xCF8
#define PCI_VALUE_PORT      0xCFC
#define PCI_ADDR            0x80000000
#define PCI_CFG_BAR0        0x10

#define PCI_NONE            0xFFFF

struct pci_device {
    uint16_t class_id;
    uint16_t subclass_id;
    uint16_t vendor_id;
    uint16_t bus;
    uint16_t slot;
    uint16_t func;
    struct pci_device *next;
};

struct pci_bar_data {
    uint64_t base;
    uint64_t io_base;
    uint32_t size;
    uint32_t type;
};

enum pci_bar_type {
    MMIO_32 = 0,
    MMIO_64 = 1,
    PIO = 2,
};

#define PCI_GET_CLASS(dev, bus, f)      (uint32_t) (pci_read_field(bus, dev, f, 0x0a, 1))
#define PCI_GET_SUBCLASS(dev, bus, f)   (uint32_t) (pci_read_field(bus, dev, f, 0x0b, 1))
#define PCI_GET_VENDOR_ID(dev, bus, f)  (uint16_t) (pci_read_field(bus, dev, f, 0, 2))

/**
 * pci_scan
 *   brief: scan all PCI buses
 */
void pci_scan(void);

/**
 * pci_fetch
 *   brief: fetch device by id
 */
struct pci_device *pci_fetch(uint32_t class, uint32_t subclass);

/**
 * pci_get_bar
 *   brief: get bar (id) for dev
 */
struct pci_bar_data pci_get_bar(uint64_t id, struct pci_device *dev);

#endif
