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

#ifndef KERNEL_MEM
#define KERNEL_MEM

#include <stdint.h>

enum mmap_type {
	INVALID                = 0,
	USABLE                 = 1,
	RESERVED               = 2,
	ACPI_RECLAIMABLE       = 3,
	ACPI_NVS               = 4,
	BAD_MEMORY             = 5,
	BOOTLOADER_RECLAIMABLE = 0x1000,
	KERNEL_AND_MODULES     = 0x1001,
	FRAMEBUFFER            = 0x1002
};

typedef struct {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t unused;
} __attribute__((packed)) mmap_entry_t;

/* Memory space structure */
typedef struct {
	uint64_t size;
	mmap_entry_t *mmap;
} __attribute__((packed)) memsp_t;

/**
 * mem_init
 *   brief: Init memory manager
 */
void mem_init(void *mmap_ptr, uint64_t size);

/**
 * get_memsp
 *   brief: Get memsp
 */
memsp_t *get_memsp();

#endif
