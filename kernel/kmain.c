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

extern void enter_usermode(void);

/* Kernel main function */
void kmain(multiboot_info_t *info)
{
	/* Init tty */
	tty_init();

	printk("eureka %s\n", KERNEL_VERSION_STRING);

	extern void *stack_top;
	init_tss(0x10, (uintptr_t) stack_top); /* Init TSS */
	init_gdt(); /* Init GDT */
	init_idt(); /* Init IDT */

	/* Set kernel mode */
	set_kernel_mode(TTY_MODE);

	/* Check if grub can give us a memory map */
	/* TODO: Detect manually */
	if (!(info->flags & (1<<6))) {
		panic("couldn't get memory map!");
	}

	/* Init mm */
	printk("\nMemory map:");
	kmem_init(info);

	/* Init paging */
	kmem_init_paging();

	/* Init all drivers */
	init_drivers();

	printk("\nWelcome! %uiMiB of RAM detected in total!",  (kmem_get_installed_memory() / 1048576) + 2);

	#ifdef BUILD_TESTS
		/* Start tests */
		do_tests();
	#endif

	/* Ring 3! */
	//enter_usermode();

	for(;;);
}
