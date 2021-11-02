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

pid_t spawn_task(char *name)
{
    static struct task *last_proc = &head_task;
    if (strlen(name) > 128) {
        err("failed to init process %i: name is too long!", last_proc->pid + 1);
        return -1;
    }

    struct task *pr = kmalloc(sizeof(struct task));
    pr->name = name;
    pr->pid = last_proc->pid + 1;    
    pr->state = SLEEPING;
    pr->cpu_state.regs.rsp = PROC_STACK_LOW;
    pr->cpu_state.page_table = generic_vmalloc(PROC_HEAP_SIZE);
    pr->next = NULL;
    last_proc->next = pr;
    last_proc = pr;

    ok("spawned process %s with PID=%i", name, pr->pid);

    return pr->pid;
}
