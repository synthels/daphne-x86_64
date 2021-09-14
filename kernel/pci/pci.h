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

#define PCI_VENDOR_ID       0x00
#define PCI_DEVICE_ID       0x02

#define PCI_SUBCLASS        0x0a
#define PCI_CLASS           0x0b
#define PCI_HEADER_TYPE     0x0e

#define PCI_SECONDARY_BUS   0x19

#define PCI_CLASS_BRIDGE    0x06

#define PCI_ADDRESS_PORT    0xCF8
#define PCI_VALUE_PORT      0xCFC
#define PCI_ADDR            0x80000000

#define PCI_NONE            0xFFFF

typedef void (*pci_callback_t)(uint32_t);

#define PCI_GET_CLASS(dev)    (int) (pci_read_field(dev, PCI_CLASS, 1) << 8)
#define PCI_GET_SUBCLASS(dev) (int) (pci_read_field(dev, PCI_SUBCLASS, 1))

/**
 * pci_search
 *   brief: search for devices of a certain type
 */
void pci_search(pci_callback_t fn, int class, int subclass);

#endif
