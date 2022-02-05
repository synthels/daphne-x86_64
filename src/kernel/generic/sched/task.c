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

static vec_t *tasks;
static pid_t pid;

void init_task(struct task *t)
{
    t->pid = pid++;
    t->context = mmu_init_context(PROC_HEAP_SIZE, PROC_STACK_LOW);
    t->state = ASLEEP;
    t->children = vector();
}

void init_task_queue(void)
{
    struct task *head_task = kmalloc(sizeof(struct task));
    init_task(head_task);
    tasks = vector();
    vec_push_ptr(tasks, head_task);
}

void init_sched(void)
{
    init_task_queue();
}

void switch_task(void)
{
    static struct task *running_task = NULL;
    static size_t tasks_index = 0;
    
    if (!running_task) {
        running_task = vec_get_as(tasks, tasks_index, struct task *);
    }
    running_task->state = ASLEEP;
    /* Also suspend children here */
    /* Warpback size if necessary */
    if (tasks_index >= tasks->size) {
        tasks_index = 0;
    }
    /* Switch to next task */
    running_task = vec_get_as(tasks, tasks_index, struct task *);
    running_task->state = RUNNING;
    mmu_switch(running_task->context);
}
