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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef MEMORY
#define MEMORY

/*
 * Our memory map is very simple on x86-32:
 *
 * +---------------+----------------------+------------------------------------+
 * | Module memory |      Map region      |            Kernel memory           |
 * +---------------+----------------------+------------------------------------+
 *
 * - Module memory: Memory occupied by kernel modules
 * - Map region: Memory occupied by frame_tree_node structures (palloc memory)
 * - Kernel memory: Usable memory
 */

#include <stdint.h>
#include <memory/init_module/init_module.h>
#include <memory/heap/kmalloc.h>
#include <memory/heap/palloc.h>

/* Init mm */
void mm_init(uint32_t upper, uint32_t lower);

/* mm getter functions */
uint32_t mm_get_upper();
uint32_t mm_get_lower();

#endif
