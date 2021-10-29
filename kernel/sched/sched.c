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

void context_switch(regs_t *regs);

void fire(struct task *task)
{
    if (task == NULL) return;
    task->state = ACTIVE;
}

void halt(struct task *task)
{
    if (task == NULL) return;
    task->state = SLEEPING;
}

void kill(struct task *task)
{
    if (task == NULL) return;
    task->state = DEAD;
}

void switch_to(struct task *tasks, size_t n)
{
    static struct task *last_task = NULL;
    if (tasks[n].state == ACTIVE) {
        halt(last_task);
        context_switch(&(tasks[n].cpu_state.regs));
        fire(&tasks[n]);
    }
    last_task = &tasks[n];
}
