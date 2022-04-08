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
 * Symbols used throughout the kernel
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * Kernel version
 */
#define KERNEL_VERSION "v0.1-alpha"

#ifdef ARCH_x86_64
    #define KERNEL_ARCH "x86-64"
    #define KERNEL_CS 0x08
    #define KERNEL_DS 0x10
    #define SYSCALL_INDEX 0x80
    #define KERNEL_STACK_SIZE 65536 /* 64KiB */
#endif

/**
 * Mark a symbol unused
 */
#define UNUSED(x) (void) (x)

/**
 * kernel_hang
 *   brief: hangs the kernel
 */
void kernel_hang(void);
