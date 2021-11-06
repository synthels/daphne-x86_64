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
static void __task_switch_internal(uint64_t ticks);

void sched_init(void)
{
    /* Hook switch_task to PIT/AHCI */
    tm_hook(&__task_switch_internal);
}

void task_run(struct task *task)
{
    if (task == NULL) return;
    task->state = ACTIVE;
    /* TSS */
    /* context_switch(task->cpu_state.regs); */
}

void task_yield(struct task *task)
{
    if (task == NULL) return;
    task->state = SLEEPING;
}

void task_kill(struct task *task)
{
    if (task == NULL) return;
    task->state = DEAD;
}

void switch_task(struct task *tasks)
{
    static struct task *first = NULL;
    static struct task *current = NULL;
    if (!first) {
        /*
         * We take the next, because the first pointer
         * in tasks is head_task while we want init
         * instead
         */
        first = tasks->next;
        current = first;
    }
    /* Yield from current task */
    task_yield(current);
    if (current->next != NULL) {
        if (current->state != DEAD) {
            task_run(current->next);
            current = current->next;
        }
    } else {
        task_run(first);
        current = first;
    }
}

static void __task_switch_internal(uint64_t ticks)
{
    UNUSED(ticks);
    switch_task(get_head_task());
}
