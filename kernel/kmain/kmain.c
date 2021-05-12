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
 * Main function
 */

#include "kmain.h"

/* Kernel main function */
void kmain(multiboot_info_t *info)
{
	tty_init();
	printk("phiOS - (C) Synthels %i, All rights reserved", KERNEL_COPYRIGHT_YEAR);

	mm_init(info->mem_upper, info->mem_lower);

	gen_lidt();
	init_modules();

	/* "Welcome" Jingle */
	spk_interface->write(400, MODULE_WRITE);
	spk_interface->write(1, MODULE_WRITE);
	spk_interface->write(0, MODULE_COMMIT);

	for(;;) {
		uint32_t key;
		if (kbd_interface->event) {
			kbd_interface->event = 0;
			kbd_interface->read(&key);
		}
	}
}
