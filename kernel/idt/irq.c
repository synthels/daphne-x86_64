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
 * Interrupt handlers
 */

#include <io/io.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/time/PIT.h>
#include <stdint.h>
#include <tty/printk.h>

#define IRQ_END outb(0x20, 0x20)

/* PIT IRQ */
void pit_irq_handler(void)
{
	pit_tick();
	IRQ_END;
}

/* Keyboard IRQ */
void kbd_irq_handler(void)
{
	kbd_read();
	IRQ_END;
}

/* syscall handler */
void syscall_handler(struct regs *regs)
{
	printk("syscall: EAX = %i", regs->eax);
	IRQ_END;
}

void generic_irq_handler(void)
{
	IRQ_END;
}
