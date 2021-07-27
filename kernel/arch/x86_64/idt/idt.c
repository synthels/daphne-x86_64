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

/* IDT */
static idt_entry_t idt[256];

/* IRQs (in IDT indices) */
static interrupt_handler_t idt_handlers[256];

/* IRQ handlers */
static interrupt_handler_t irq_handlers[256];

/* Install an IRQ handler to the IDT */
void idt_install_handler(void (*handler)(void), int i)
{
    idt_handlers[i] = handler;
}

/* Install all handlers */
void idt_install_handlers()
{
    ISR(isr0, 0);
    ISR(isr1, 1);
    ISR(isr2, 2);
    ISR(isr3, 3);
    ISR(isr4, 4);
    ISR(isr5, 5);
    ISR(isr6, 6);
    ISR(isr7, 7);
    ISR(isr8, 8);
    ISR(isr9, 9);
    ISR(isr10, 10);
    ISR(isr11, 11);
    ISR(isr12, 12);
    ISR(isr13, 13);
    ISR(isr14, 14);
    ISR(isr15, 15);
    ISR(isr16, 16);
    ISR(isr17, 17);
    ISR(isr18, 18);
    ISR(isr19, 19);
    ISR(isr20, 20);
    ISR(isr21, 21);
    ISR(isr22, 22);
    ISR(isr23, 23);
    ISR(isr24, 24);
    ISR(isr25, 25);
    ISR(isr26, 26);
    ISR(isr27, 27);
    ISR(isr28, 28);
    ISR(isr29, 29);
    ISR(isr30, 30);
    ISR(isr31, 31);
    /* syscall (TODO: this needs to be in userspace) */
    ISR(isr128, 128);

    /* PIT */
    IRQ(irq0, pit_irq_handler, 0);
    /* Keyboard */
    IRQ(irq1, kbd_irq_handler, 1);
}

/* Generate & load an IDT */
void init_idt(void)
{
    extern int load_idt();
    idtp idt_ptr;

    idt_install_handlers();
    remap_pic();

    /* Fill the IDT */
    for (int i = 0; i < 129; i++) {
        uintptr_t base = (uintptr_t) idt_handlers[i];
        idt[i].base_low  = (base & 0xFFFF);
        idt[i].base_mid  = (base >> 16) & 0xFFFF;
        idt[i].base_high = (base >> 32) & 0xFFFFFFFF;
        idt[i].selector = 0x08;
        idt[i].zero = 0;
        idt[i].pad = 0;
        idt[i].flags = 0x8e; /* kernel space only (OR 0) */
    }

    idt_ptr.limit = sizeof(idt);
    idt_ptr.base = (uintptr_t) &idt;

    asm volatile (
        "lidt %0\n"
        : : "m"(idt_ptr)
    );
}

void isr_handler(regs_t *r)
{
    extern void syscall_handler(regs_t *r);
    const char *exceptions[] = {
        "division by zero",
        "debug",
        "NMI",
        "breakpoint",
        "invalid",
        "out of bounds",
        "invalid opcode",
        "invalid",
        "double fault",
        "coprocessor segment overrun",
        "bad TSS",
        "segment not present",
        "stack fault",
        "GPF",
        "page fault",
        "unknown interrupt",
        "invalid",
        "alignment check",
        "machine check",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved",
        "reserved"
    };

    /* Exception/IRQ?? */
    if (r->int_no < 32) {
        panic(exceptions[r->int_no]);
    } else if (r->int_no == __SYSCALL_INDEX) {
        syscall_handler(r);
        return;
    } else {
        (*irq_handlers[r->int_no - 32])();
    }

    /* Slave EOI */
    if (r->int_no >= 40)
       outb(0xA0, 0x20);
    /* Master EOI */
    outb(0x20, 0x20);
}
