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
 * Serial I/O
 */

#include "io.h"

void outb(uint16_t port, uint8_t val)
{
	asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outl(uint16_t port, uint8_t val)
{
	asm volatile ( "outl %%eax, %%dx": :"d" (port), "a" (val) );
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile ( "inb %1, %0"
		: "=a"(ret)
		: "Nd"(port) );
	return ret;
}

uint8_t inl(uint16_t port)
{
	uint8_t ret;
	asm volatile ( "inl %%dx, %%eax"
		: "=a"(ret)
		: "d"(port) );
	return ret;
}

void io_wait(void)
{
	asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}
