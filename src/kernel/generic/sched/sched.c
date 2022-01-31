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
 * Task scheduling
 */

#include "sched.h"

static void __task_switch_internal(regs_t *regs, uint64_t ticks);

declare_lock(sched_lock);

void sched_init(void)
{
    /* Hook switch_task to PIT/AHCI */
    tm_hook(&__task_switch_internal);
    init_stash();
}

void task_run(struct task *task)
{
    if (task == NULL) return;
    task->state = ACTIVE;

    /* Context switch (TODO: specific to x64...) */
    Q_vswitch(task->context->page_table);
    Q_swapregs(task->context->regs);
}

void task_yield(struct task *task)
{
    if (task == NULL) return;
    task->state = SLEEPING;
}

void task_kill(struct task *task)
{
    if (task == NULL) return;
    stash_context(task->context);
    task->state = DEAD;
}

void switch_task(struct task *tasks)
{
    lock(&sched_lock);
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
        /* TODO: Remove dead tasks */
        if (current->state != DEAD) {
            task_run(current->next);
            current = current->next;
        }
    } else {
        task_run(first);
        current = first;
    }
    unlock(&sched_lock);
}

static void __task_switch_internal(regs_t *regs, uint64_t ticks)
{
    UNUSED(ticks);
    UNUSED(regs);
    switch_task(get_head_task());
}
