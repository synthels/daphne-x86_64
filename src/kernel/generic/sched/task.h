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

#include <lib/log.h>
#include <lib/lock.h>
#include <lib/list.h>
#include <lib/string.h>

#include <generic/malloc/malloc.h>
#include <generic/malloc/mmu.h>
#include <generic/time/clock.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/x64.h>
    #include <arch/x86_64/pmm.h>
    #include <arch/x86_64/vmm.h>
    #include <arch/x86_64/smp.h>
#endif

#define SCHED_SINGLE_CORE_MAX_TASKS 4

typedef uint16_t pid_t;
enum task_state_t {
    DEAD,
    SUSPENDED,
    ASLEEP,
    RUNNING
};

struct task {
    const char *name;
    pid_t pid;
    struct context *context;
    struct task_args *args;     /* TODO */
    enum task_state_t state;    /* Task state */
    int assigned_to_cpu;        /* id of cpu this task is assigned to */
    struct task *next;          /* Next task */
    struct list *children;      /* Task children */
};

struct task_args {
    int argc;
    char *argv;
};

struct processor {
    /**
     * Tasks assigned to this CPU. If task is NULL,
     * this CPU sits idly
     */
    struct task *root;
    struct task *running_task;
    int cpu_id;
    int lapic_id;
    bool is_bsp; /* set if this cpu is the bsp */

    /* TODO: not used */
    int  cpu_model;
    int  cpu_family;
    char cpu_model_name[48];
    const char *cpu_manufacturer;
};

/**
 * this_core
 *   brief: points to the core running this code
 */
static struct processor __seg_gs *const this_core = NULL;

/**
 * sched_init
 *   brief: init scheduler
 */
void sched_init(void);

/**
 * sched_create_task
 *   brief: create task
 */
struct task *sched_create_task(const char *name);

/**
 * sched_queue
 *   brief: queue task
 */
void sched_queue(struct task *t);

/**
 * switch_task
 *   brief: switch task
 */
void switch_task(regs_t *r, uint64_t jiffies);
