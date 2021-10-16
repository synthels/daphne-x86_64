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
 * Task scheduling
 */

#include "sched.h"

void fire(struct proc *task)
{
    task->state = ACTIVE;
}

void halt(struct proc *task)
{
    task->state = SLEEPING;
}

void kill(struct proc *task)
{
    task->state = DEAD;
}

void switch_task(struct proc *tasks)
{
    /* C99 really doesn't want me to be elegant here */
    static bool first_call = true;
    static struct proc *curr_task;
    static struct proc *first;
    /* Task killed */
    if (curr_task->state == DEAD) return;
    if (first_call) {
        curr_task = tasks;
        first = tasks;
        first_call = true;
    }
    halt(curr_task);
    if (curr_task->next != NULL) {
        /* Check if next task is dead */
        if (curr_task->next->state != DEAD) fire(curr_task->next);
        curr_task = curr_task->next;
    } else {
        fire(first);
        curr_task = first;
    }
}
