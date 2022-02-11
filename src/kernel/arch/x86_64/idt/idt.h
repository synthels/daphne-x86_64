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
#include <generic/forbia/panic.h>
#include <generic/forbia/kernel.h>

#include <arch/x86_64/x64.h>
#include <arch/x86_64/pic.h>
#include <arch/x86_64/smp.h>

#define ISR(isr, n) extern void isr(void); \
    idt_install_handler(isr, n)

#define IRQ(irq, fn, n) extern void irq(void); \
    extern void fn(void); \
    irq_handlers[n] = fn; \
    idt_install_handler(irq, n + 32)

typedef void (*interrupt_handler_t)(void);

/* IDT entry */
typedef struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t pad;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed)) idtp;

/**
 * idt_install_irq_handler
 *   brief: Install a new IRQ handler
 *   parameters:
 *     - handler: pointer to handler
 *     - i:       IRQ
 */
void idt_install_irq_handler(void (*handler)(void), int i);

/**
 * idt_init
 *   brief: Load the kernel IDT
 */
void idt_init(void);
