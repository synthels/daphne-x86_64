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
 * Tasks & threads
 */

#include "task.h"

static struct task root = {0, NULL, ASLEEP, NULL, NULL};
static struct task *task_ptr; /* Current task */
static bool _multicore;
static bool first_task = true;
static pid_t pid;

/**
 * @brief Initialise task
 *
 * Allocate a context for this task
 */
void init_task(struct task *t)
{
    /* Init first task */
    if (first_task) {
        task_ptr = &root;
        first_task = false;
    }
    /* Fill in all the fields */
    t->pid = pid++;
    t->context = mmu_init_context(PROC_HEAP_SIZE, PROC_STACK_LOW);
    t->state = ASLEEP;
    t->children = vector();
    /* Add task to the linked list */
    task_ptr->next = t;
    task_ptr = t;
}

void init_sched(void)
{
    /* Check if we run within a multicore system */
    struct smp_cpus *cpus = smp_get_cores();
    if (cpus->size < 2) {
        _multicore = false;
    } else {
        _multicore = true;
    }
}

void switch_task(void)
{

}
