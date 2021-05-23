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
 * IDT
 */

#include "idt.h"

/* Install an IRQ handler to the IDT */
void idt_install_irq_handler(void (*handler)(void), int i)
{
	irq_handlers[i] = handler;
}

/* Install all handlers */
void idt_install_handlers()
{
	extern void irq0(void);
	extern void irq1(void);
	extern void _syscall(void);
	extern void generic_irq(void);

	/* PIT */
	idt_install_irq_handler(irq0, 0);
	/* Keyboard */
	idt_install_irq_handler(irq1, 1);
	/* syscall */
	idt_install_irq_handler(_syscall, 128);
}

/* Generate & load an IDT */
void init_idt(void)
{
	extern int load_idt();
	uint32_t irq_addresses[129];
	uint32_t idt_address;
	unsigned long idt_ptr[2];

	/* Install the handlers */
	idt_install_handlers();
	/* Remap the PIC */
	remap_pic();

	/* Fill the IDT */ 
	for (int i = 0; i < 129; i++) {
		irq_addresses[i] = (uint32_t) irq_handlers[i];
		IDT_buffer[32 + i].offset_lowerbits = irq_addresses[i] & 0xffff;
		IDT_buffer[32 + i].selector = 0x08;
		IDT_buffer[32 + i].zero = 0;
		IDT_buffer[32 + i].type_attr = 0x8e;
		IDT_buffer[32 + i].offset_higherbits = (irq_addresses[i] & 0xffff0000) >> 16;
	}

	idt_address = (uint32_t) IDT_buffer;
	idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16;
	load_idt(idt_ptr);
}
