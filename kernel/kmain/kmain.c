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

/* Kernel main function */
void kmain(multiboot_info_t *info)
{
	/* Init tty */
	tty_init();

	printk("phiOS %s", KERNEL_VERSION_STRING);

	/* Set kernel mode */
	set_kernel_mode(TTY_MODE);

	printk("\nEnable IRQs");
	/* Init IDT */
	init_idt();

	/* Check if grub can give us a memory map */
	/* TODO: Detect manually */
	if (!(info->flags & (1<<6))) {
		panic("couldn't get memory map!");
	}

	/* Init page directory */
	/* TODO: Set up proper id paging */
	/* init_page_directory(); */

	printk("\nScan memory");
	/* Init mm */
	mm_init((mmap_entry_t *) info->mmap_addr, info->mmap_length);

	/* Init all drivers */
	init_drivers();

	#ifdef BUILD_TESTS
		/* Start tests */
		do_tests();
	#endif

	for(;;);
}
