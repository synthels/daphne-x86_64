/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <generic/forbia/kernel.h>
#include <generic/forbia/types.h>
#include <generic/forbia/panic.h>
#include <generic/memory/mem.h>

#include "pmm.h"

#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))
#define MEM_PHYS_OFFSET (size_t) 0xffff800000000000
#define MEM_KERN_OFFSET (size_t) 0xffffffff80000000
#define MMU_CEIL(a, b) (((a) + ((b) - 1)) / (b))

#define HEAP_PAGES 500 /* Pages in the heap (2MiB) */

#define FLAGS_READ_WRITE 0x03

#define GB 0x40000000UL

/**
 * init_vmm
 *   brief: init vmm
 */
void vmm_init(void);

/**
 * init_vmm
 *   brief: get PML4
 */
uint64_t *vmm_get_pml4(void);

/**
 * pml4_map_page
 *   brief: Map single page without access to the PML
 */
void pml4_map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

/**
 * map_page
 *   brief: Map single page
 */
void map_page(uint64_t *pml, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

/**
 * vmalloc
 *   brief: Create new virtual address space of size n
 */
uint64_t *vmalloc(size_t n);

/**
 * vswitch
 *   brief: Switch pml
 */
void vswitch(uint64_t *pml);
