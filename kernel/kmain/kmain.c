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
 * Main function
 */

#include "kmain.h"
#include <memory/malloc.h>

static void info_begin(const char *str)
{
	tty_puts("------------", VGA_COLOR_LIGHT_RED);
	tty_puts(str, VGA_COLOR_LIGHT_RED);
	tty_puts("------------", VGA_COLOR_LIGHT_RED);
}

/* Kernel main function */
void kmain(multiboot_info_t *info)
{
	/* Init tty */
	tty_init();

	printk("phiOS - (C) Synthels %i, All rights reserved", KERNEL_COPYRIGHT_YEAR);

	/* Set kernel mode */
	kernel_mode = TTY_MODE;

	/* Check if grub can give us a memory map */
	/* TODO: Detect manually */
	if (!(info->flags & (1<<6))) {
		panic("couldn't get memory map!");
	}

	/* Init page directory */
	/* TODO: Set up proper id paging */
	/* init_page_directory(); */

	/* Init mm */
	info_begin("Memory info");
	mm_init((mmap_entry_t *) info->mmap_addr, info->mmap_length);

	/* Init IDT */
	init_idt();

	/* Init all drivers */
	init_drivers();

	for(;;);
}
