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

#include <arch/x86_64/vmm.h>
#include <arch/x86_64/context.h>
#include <arch/x86_64/cpu.h>

#include <generic/memory/mem.h>

#include <stdint.h>
#include <stddef.h>

/**
 * mmu_alloc
 *   brief: allocate n bytes on the heap
 */
void *mmu_alloc(size_t n);

/**
 * mmap_current
 *   brief: memory map region in current virtual space
 */
void mmap_current(uintptr_t base, size_t pages);

/**
 * mmap
 *   brief: memory map region
 */
void mmap(void *pml, uintptr_t base, size_t pages);

/**
 * mmu_vmalloc
 *   brief: allocate page directory
 */
uint64_t *mmu_vmalloc(size_t n);

/**
 * mmu_vswitch
 *   brief: switch page directory
 */
void mmu_vswitch(uint64_t *pml);

/**
 * mmu_init_context
 *   brief: init virtual context
 */
struct context *mmu_init_context(size_t heap, uint64_t stack);

/**
 * mmu_switch
 *   brief: switch context
 */
void mmu_switch(struct context *context);
