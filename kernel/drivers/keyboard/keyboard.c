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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR See the
 * GNU General Public License for more details.
 *
 * Simple PS/2 keyboard driver
 */

#include "keyboard.h"

static int kbd_enabled = 0;
static int kbd_last_key_pressed;
static uint8_t kbd_last_key;

/* Key event */
static int kbd_keyev = 0;

/* Init keyboard */
void kbd_init(void)
{
	/* Enable keyboard IRQ */
	outb(0x21, 0xfd);
	kbd_enabled = 1;
}

/* Read 0x60 */
void kbd_read()
{
	if (kbd_enabled) {
		kbd_keyev = 1;
		kbd_last_key = inb(0x60);
		kbd_last_key_pressed = !(kbd_last_key & 0x80);
		return;
	}
	/* Keyboard is disabled */
	/* Read once again from here, so the PIC doesn't think we're stupid
	   if the keyboard is disabled */
	kbd_last_key = '\0';
	inb(0x60);
}

/* Check if keyboard is enabled */
int kbd_is_enabled(void)
{
	return kbd_enabled;
}

/* Enable/disable keyboard */
void kbd_set_enabled(int enabled)
{
	kbd_enabled = enabled;
}

/* Return last key */
void kbd_get_last_key(uint8_t *key, int *pressed)
{
	*key = kbd_last_key;
	*pressed = kbd_last_key_pressed;
}

/* Check if there is a key event */
int kbd_get_event()
{
	return kbd_keyev;
}

uint8_t kbd_translate(uint8_t key, uint8_t layout[])
{
	return layout[key];
}

/* Acknowledge that keyboard event was read */
/* This is fine, since there will always be exactly one recepient of this event,
   the kernel. The kernel can then communicate to user-space */
void kbd_ack()
{
	kbd_keyev = 0;
}
