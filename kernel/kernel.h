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

#ifndef KERNEL
#define KERNEL

#include <stddef.h>
#include <stdint.h>
#include <vid/console/con.h>

/**
 * Kernel version string
 *   fmt: vMajor.minor-Codename 
 */
#define KERNEL_VERSION_STRING "v1.0-Alpha"

#ifdef ARCH_x86_64
	#define KERNEL_ARCH_STRING "x86-64"
#endif

/**
 * x86 registers struct
 */
struct regs {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
};
typedef struct regs regs_t;

/**
 * Mark a symbol unused
 */
#define UNUSED(x) (void) (x)

/**
 * errcode_t
 *   brief: function return type 
 */
typedef int errcode_t;

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

/**
 * <stbool.h>
 */
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

#endif
