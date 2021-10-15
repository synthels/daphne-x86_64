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

#ifndef KERNEL_PROC
#define KERNEL_PROC

#include <libk/printk.h>
#include <libk/string.h>

#include <malloc/malloc.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/pmm.h>
    #include <arch/x86_64/vmm.h>
#endif

typedef int16_t pid_t;

enum proc_state {
    ACTIVE = 0,
    SLEEPING = 1,
    DEAD = 2
};

struct proc_cpu_state {
    uint64_t *page_table;
    regs_t regs;
};

struct proc {
    pid_t pid;
    pid_t parent_pid;
    int state;
    struct proc_cpu_state cpu_state;
    char *name;
    struct proc *next;
};

/**
 * new_process
 *   brief: create new process
 */
pid_t new_process(char *name);

#endif
