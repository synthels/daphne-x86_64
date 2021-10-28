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
 * Symbols used throughout the kernel
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * Kernel version
 *   fmt: vMajor.minor-Codename 
 */
#define KERNEL_VERSION "v0.1-Alpha"

#ifdef ARCH_x86_64
    #define KERNEL_ARCH "x86-64"
    #define __KERNEL_CS 0x8
    #define __KERNEL_DS 0x10
    #define __SYSCALL_INDEX 0x80
#endif

/**
 * Mark a symbol unused
 */
#define UNUSED(x) (void) (x)

/**
 * EINVAL
 *   brief: invalid argument error
 */
#define EINVAL -1

/**
 * NOERR
 *   brief: No error
 */
#define NOERR 0

/**
 * kern_queue_calls
 *   brief: Calls f1 before f2 if cond is true,
 *          calls f1 after f2 otherwise
 */
#define kern_queue_calls(f1, f2, cond) if (cond) { f1; f2; } else { f2; f1; }

/**
 * set_kernel_mode
 *   brief: Set current kernel mode
 *   parameters:
 *     - mode: new kernel mode
 */
void set_kernel_mode(int mode);

/**
 * set_kernel_mode
 *   brief: Get current kernel mode
 */
int get_kernel_mode(void);

/**
 * kernel_hang
 *   brief: hangs the kernel
 */
void kernel_hang(void);

/**
 * TTY_MODE
 *   brief: tty mode puts the kernel in a state where
 *          it can only write to the screen, but not
 *          accept user input.
 */
#define TTY_MODE 1

/**
 * USR_MODE
 *   brief: user mode puts the kernel in a state where
 *          it can perform all normal 
 *          user I/O operations.
 */
#define USR_MODE 2

/* printk escape sequences */
#define KERN_OVERWRITE "\xff[0000000"
