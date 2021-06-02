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
 */

#ifndef MEMORY
#define MEMORY

#include <stdint.h>
#include <tty/printk.h>
#include <multiboot.h>

#define MiB(n) 1048576 * n 
#define KERN_END (uint32_t) &kend

struct mmap_entry {
	uint32_t size;
	uint32_t base_addr_low, base_addr_high;
	uint32_t length_low, length_high;
	uint32_t type;
}  __attribute__((packed));

typedef struct mmap_entry mmap_entry_t;

/* Init mm */
void kmem_init(multiboot_info_t *info);

/*
 * mm getters
 */
mmap_entry_t *kmem_get_kernel_mmap();
size_t kmem_get_kmmap_size();
uint32_t kmem_get_ram();

#define MEMORY_AVAILABLE 1
#define MEMORY_RESERVED 2
#define MEMORY_ACPI 3
#define MEMORY_NVS 4
#define MEMORY_BADRAM 5
#define MEMORY_INVALID 6

#endif
