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
 * Kernel main function
 */

#include "kmain.h"

extern void enter_usermode(void);

/* Kernel main function */
void kmain(efi_mmap_t *mmap)
{
	/* Init tty */
	tty_init();

	printk("daphne %s (%s) \n", KERNEL_VERSION_STRING, KERNEL_ARCH_STRING);

	uint32_t esp;
	asm volatile("mov %%rsp, %0" : "=r"(esp));
	init_tss(0x10, esp); /* Init TSS */
	init_gdt(); /* Init GDT */
	init_idt(); /* Init IDT */

	/* Set kernel mode */
	set_kernel_mode(TTY_MODE);

	/* Init mm */
	printk("\nMemory map:");
	/* TODO: pass memory map only, in order to make
	   supporting x64 easier */
	kmem_init(mmap);

	/* Init paging */
	kmem_init_paging();

	/* Init essential devices */
	dev_init_essentials();

	printk("total_ram=%uiMB",  (kmem_get_installed_memory() / 1048576) + 2);

	#ifdef BUILD_TESTS
		/* Start tests */
		do_tests();
	#endif

	/* Ring 3! */
	enter_usermode();
	printk("\nHello user mode! :)");

	for(;;);
}
