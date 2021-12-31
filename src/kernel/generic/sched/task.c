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
 * General process management
 */

#include <generic/sched/task.h>

static struct task head_task = {
    0, 0, ACTIVE, 0, "", NULL
};

static struct context_stash stash;

struct task *get_head_task(void)
{
    return &head_task;
}

void init_stash(void)
{
    stash.size = 0;
    stash.contexts = kmalloc(STASH_SIZE * sizeof(uint64_t *));
}

context_t *get_stashed_context(void)
{
    static size_t idx = 0;
    if (idx >= stash.size) {
        idx = 0;
    }
    return (context_t *) stash.contexts[idx++];
}

void stash_context(context_t *context)
{
    if (stash.size >= STASH_SIZE) {
        stash.size = 0;
    }
    stash.contexts[stash.size++] = context;
}

pid_t ktask_run(char *name)
{
    struct task *current = &head_task;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = kmalloc(sizeof(struct task));
    current->next->name = name;
    current->next->pid = current->pid + 1;
    current->next->state = SLEEPING;
    if (!stash.size) {
        current->next->context = Q_init_context(PROC_HEAP_SIZE, PROC_STACK_LOW);
    } else {
        /* Grab a page table from the stash */
        current->next->context = get_stashed_context();
    }
    current->next->next = NULL;

    ok("ktask_run: created task %s", name);

    return current->next->pid;
}
