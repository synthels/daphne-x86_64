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

#include "process.h"

struct proc initial_proc = {
    0, 0, ACTIVE, {0}, "", NULL
};

pid_t new_process(char *name)
{
    static struct proc *last_proc = &initial_proc;
    if (strlen(name) > 128) {
        err("failed to init process %i: name is too long!", last_proc->pid + 1);
        return -1;
    }

    struct proc *pr = kmalloc(sizeof(struct proc));
    pr->name = name;
    pr->pid = last_proc->pid + 1;    
    pr->state = ACTIVE;
    pr->cpu_state.regs.rsp = PROC_STACK_LOW;
    pr->cpu_state.page_table = vmalloc(PROC_HEAP_SIZE);
    pr->next = NULL;
    last_proc->next = pr;
    last_proc = pr;

    ok("spawned process %s with PID=%i", name, pr->pid);

    return pr->pid;
}
