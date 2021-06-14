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

#ifndef KERNEL_x86_32_IDT
#define KERNEL_x86_32_IDT

#include "PIC.h"
#include <stdint.h>
#include <tty/printk.h>
#include <logger/panic.h>

#define ISR(isr, n) extern void isr(void); \
	idt_install_raw_handler(isr, n)

#define IRQ(irq, n) extern void irq(void); \
	idt_install_irq_handler(irq, n)

#define IRQ_HANDLERS 4

/* IDT entry */
struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
} __attribute__((packed));

/**
 * idt_install_irq_handler
 *   brief: Install a new IRQ handler
 *   parameters:
 *     - handler: pointer to handler
 *     - i:       IRQ
 */
void idt_install_irq_handler(void (*handler)(void), int i);

/**
 * init_idt
 *   brief: Load the kernel IDT
 */
void init_idt(void);

#endif
