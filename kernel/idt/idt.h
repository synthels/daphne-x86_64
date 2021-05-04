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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef KERNEL_x86_IDT
#define KERNEL_x86_IDT

#include <idt/PIC.h>
#include <stdint.h>

/* Single IDT entry */
struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
} __attribute__((packed));

/* IDT structure */
struct IDT_entry IDT_buffer[256];

/* IRQ handlers */
void *irq_handlers[16];

/* Install an IRQ handler to the IDT */
void idt_install_irq_handler(void (*handler)(void), int i);

/* Initialise the IDT */
void gen_lidt(void);

#endif
