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
 *
 * Kernel types
 */

#pragma once

#define bool _Bool

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

typedef struct {
    uint32_t type;
    uint32_t pad;
    uint64_t phys_start;
    uint64_t virt_start;
    uint64_t pages;
    uint64_t attr;
} efi_memory_descriptor_t;

typedef struct {
    efi_memory_descriptor_t *map;
    uint64_t size;
    uint64_t desc_size;
} efi_mmap_t;

typedef struct {
    uint64_t fb_base;
    uint64_t pps;
    uint64_t width;
    uint64_t height;
} efi_gop_info_t;

typedef struct {
    efi_mmap_t *mmap;
    efi_gop_info_t *gop;
} efi_info_t;
