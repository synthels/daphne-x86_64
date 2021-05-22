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
 * PCI
 */

#include "pci.h"

MODULE_NAME("pci");
MODULE_AUTH("synthels");

uint16_t pci_cfg_readw(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
	uint32_t addr;
	uint32_t lbus = (uint32_t) bus;
	uint32_t lslot = (uint32_t) slot;
	uint32_t lfunc = (uint32_t) func;
	uint16_t tmp = 0;

	addr = (uint32_t) (
		(lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t) 0x80000000)
	);

	outl(0xcfc, addr);
	/* Read data */
	tmp = (uint16_t) ((inl(0xcfc) >> ((offset & 2) * 8)) & 0xffff);
	return tmp;
}
