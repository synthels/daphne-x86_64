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

#ifndef KERNEL_
#define KERNEL_

#include <stddef.h>
#include <stdint.h>

#define KERNEL_COPYRIGHT_YEAR 2021

struct regs {
	uint32_t _unused, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t eip, cs, eflags, useresp, ss;
};

typedef struct regs regs_t;

#define UNUSED(x) (void) (x)
#define WORD sizeof(size_t)

#define SIGOK 0x0
#define SIGERR 0xA
#define SIGWARN 0x14

/*
 * Kernel mode works only with a VGA text mode
 * and no user input
 */
#define TTY_MODE 0x10

/*
 * OS mode supports user input
 * and may or may not have set up a proper video mode
 */
#define GFX_MODE 0x11

/* Current kernel mode */
int kernel_mode;

#endif
