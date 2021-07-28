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

#ifndef KERNEL_x86_64_VMM
#define KERNEL_x86_64_VMM

#include <stddef.h>
#include <stdint.h>
#include <forbia/kernel.h>
#include <forbia/types.h>
#include <forbia/panic.h>
#include <mem/mem.h>

#include "pmm.h"

#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))
#define MEM_PHYS_OFFSET (size_t) 0xffff800000000000
#define MEM_KERN_OFFSET (size_t) 0xffffffff80000000
#define MMU_CEIL(a, b) (((a) + ((b) - 1)) / (b))

#define HEAP_PAGES 50 /* Pages in the heap (200KiB) */

#define VMM_DEFAULT_PERMISSIONS 0x03 /* wr supervisor only */

/**
 * init_vmm
 *   brief: init vmm
 */
void vmm_init(void);

/**
 * map_page
 *   brief: Map single page
 */
void map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

#endif
