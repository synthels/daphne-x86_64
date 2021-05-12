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
 *
 * Interrupt handlers
 */

#include <io/io.h>
#include <modules/keyboard/keyboard.h>
#include <modules/time/PIT.h>
#include <stdint.h>
#include <tty/printk.h>

#define IRQ_END outb(0x20, 0x20)

void irq0_handler(void)
{
	pit_tick();
	IRQ_END;
}

/* Keyboard IRQ */
void irq1_handler(void)
{
	kbd_read();
	IRQ_END;
}

void irq2_handler(void)
{
	IRQ_END;
}

void irq3_handler(void)
{
	IRQ_END;
}

void irq4_handler(void)
{
	IRQ_END;
}

void irq5_handler(void)
{
	IRQ_END;
}

void irq6_handler(void)
{
	IRQ_END;
}

void irq7_handler(void)
{
	IRQ_END;
}

void irq8_handler(void)
{
	IRQ_END;
}

void irq9_handler(void)
{
	IRQ_END;
}

void irq10_handler(void)
{
	IRQ_END;
}

void irq11_handler(void)
{
	IRQ_END;
}

void irq12_handler(void)
{
	IRQ_END;
}

void irq13_handler(void)
{
	IRQ_END;
}

void irq14_handler(void)
{
	IRQ_END;
}

void irq15_handler(void)
{
	IRQ_END;
}
