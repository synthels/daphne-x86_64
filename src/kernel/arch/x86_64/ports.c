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
 *
 * Serial I/O
 */

#include "ports.h"

void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outl(uint16_t port, uint32_t val)
{
    asm volatile ( "outl %%eax, %%dx" : : "d" (port), "a" (val) );
}

void outs(uint16_t port, uint16_t val)
{
    asm volatile ( "outw %%ax, %%dx" : : "d" (port), "a" (val) );
}

void outd(uint16_t port, uint16_t val)
{
	asm volatile ("out %1, %0" : : "dN" (port), "a" (val));
}

void outsl(int port, const void *addr, int cnt)
{
    asm volatile("cld; rep outsl" :
                "=S" (addr), "=c" (cnt) :
                "d" (port), "0" (addr), "1" (cnt) :
                "cc");
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm ("inw %%dx, %%ax" 
        : "=a" (ret) 
        : "d" (port) );
    return ret;
};

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
        : "=a"(ret)
        : "dN"(port) );
    return ret;
}

uint32_t inl(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "inl %%dx, %%eax"
        : "=a"(ret)
        : "d"(port) );
    return ret;
}

uint16_t ins(uint16_t port)
{
    uint16_t ret;
    asm volatile ( "inl %%dx, %%eax" 
        : "=a" (ret) 
        : "dN" (port) );
    return ret;
}

uint16_t ind(uint16_t port)
{
    uint16_t ret;
    asm volatile("in %1, %0" : "=a" (ret) : "d" (port));
    return ret;
}

void io_wait(void)
{
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}
