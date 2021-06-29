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
#include <panic.h>
#include <kernel.h>

#include "malloc.h"

#ifdef ARCH_x86_32
	#include <arch/x86_32/vmm.h>
#endif

#ifdef ARCH_x86_64
typedef uint64_t addr_t;
#endif

#ifdef ARCH_x86_32
typedef uint32_t addr_t;
#endif

#define MiB(n) 1048576 * n 
#define KiB(n) 1024 * n

#ifdef ARCH_x86_64
#define KERN_END (uint64_t) &kend
#endif

#ifdef ARCH_x86_32
#define KERN_END (uint32_t) &kend
#endif

#define PAGE_SIZE 4096

#define fast_ceil(x, y) ((long long) x + y - 1) / y
#define kmem_align(n) 32 * fast_ceil(n, 32)
#define kmem_page_align(n) PAGE_SIZE * fast_ceil(n, PAGE_SIZE)
#define next_desc(ptr, size) ((efi_memory_descriptor_t *) (((uint8_t *) ptr) + size))

/* Paging types */
typedef uint32_t pte_t;
typedef uint32_t pdir_t;

/**
 * kmem_init
 *   brief: init mm  
 */
void kmem_init(efi_mmap_t *mmap);

/*
 * kmem_get_kernel_mmap
 *   brief: get kernel memory map
 */
efi_memory_descriptor_t *kmem_get_kernel_mmap(void);

/**
 * kmem_get_kmmap_size
 *   brief: get kernel memory map size
 */
size_t kmem_get_kmmap_size(void);

/**
 * kmem_get_installed_memory
 *   brief: get total installed memory in bytes
 */
uint32_t kmem_get_installed_memory(void);

#define MEMORY_AVAILABLE 7
#define MEMORY_RESERVED 0
#define MEMORY_ACPI 9
#define MEMORY_NVS 10
#define MEMORY_INVALID 99

#endif
