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
#include <io/io.h>

#include <forbia/types.h>
#include <libk/printk.h>
#include <malloc/malloc.h>

#define PCI_VENDOR_ID       0x00
#define PCI_DEVICE_ID       0x02

#define PCI_HEADER_TYPE     0x0e

#define PCI_SECONDARY_BUS   0x19

#define PCI_CLASS_BRIDGE    0x06

#define PCI_ADDRESS_PORT    0xCF8
#define PCI_VALUE_PORT      0xCFC
#define PCI_ADDR            0x80000000

#define PCI_NONE            0xFFFF

#define PCI_SUBCLASS        0x0a
#define PCI_CLASS           0x0b

struct pci_device {
    uint16_t class_id;
    uint16_t subclass_id;
    uint16_t vendor_id;
    uint16_t bus;
    uint16_t slot;
    uint16_t func;
    struct pci_device *next;
};

#define PCI_GET_CLASS(dev, bus, f)      (uint16_t) (pci_read_field(bus, dev, f, 0x8, 2) >> 24)
#define PCI_GET_SUBCLASS(dev, bus, f)   (uint16_t) (pci_read_field(bus, dev, f, 0x8, 2) >> 16)
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

#endif
