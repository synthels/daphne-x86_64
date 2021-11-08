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

#include <sched/task.h>

static struct task head_task = {
    0, 0, ACTIVE, {0}, "", NULL
};

struct task *get_head_task(void)
{
    return &head_task;
}

pid_t ktask_run(char *name)
{
    struct task *current = &head_task;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = kmalloc(sizeof(struct task));
    current->next->cpu_state.regs = kmalloc(sizeof(regs_t));
    current->next->name = name;
    current->next->pid = current->pid + 1;
    current->next->state = SLEEPING;
    current->next->cpu_state.regs->rsp = PROC_STACK_LOW;
    current->next->cpu_state.page_table = Q_vmalloc(PROC_HEAP_SIZE);
    current->next->next = NULL;

    ok("ktask_run: created task %s", name);

    return current->next->pid;
}
