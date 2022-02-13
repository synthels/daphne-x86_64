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
 */

#pragma once

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
void outl(uint16_t port, uint32_t val);
void outs(uint16_t port, uint16_t val);
void outsl(int port, const void *addr, int cnt);
void outd(uint16_t port, uint16_t val);

uint8_t inb(uint16_t port);
uint16_t ind(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

void io_wait(void);
