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
 * PS/2 keyboard driver
 */

#include "keyboard.h"

static int kbd_last_key_pressed;
static uint8_t kbd_last_key;

int kbd_no_write(uint32_t *data)
{
	UNUSED(data);
	return DRIVER_NO_WRITE;
}

/* Init keyboard */
void kbd_init(void)
{
	kbd_send_command(0xfd);
	kbd_interface = (struct driver_interface *) lmmap(sizeof(struct driver_interface));

	kbd_interface->enabled = 1;
	kbd_interface->event = 0;
	kbd_interface->write = &kbd_no_write;
	kbd_interface->read = &kbd_get_last_key;
}

/* Read 0x60 */
void kbd_read()
{
	if (kbd_interface->enabled) {
		kbd_last_key = inb(0x60);
		kbd_last_key_pressed = !(kbd_last_key & 0x80);
		kbd_interface->event = 1;
		return;
	}
	/* Read once again from here, so the PIC doesn't think we're stupid
	   if the keyboard is disabled */
	kbd_last_key = '\0';
	inb(0x60);
}

/* Return last key */
void kbd_get_last_key(uint32_t *key)
{
	*key = kbd_last_key | kbd_last_key_pressed;
}
