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
 * Daphne's scheduler
 */

#include "task.h"

static struct task root = {"", 0, NULL, ASLEEP, -1, NULL, NULL}; /* Root task */
static struct smp_cpus *cpus; /* CPUs */
static bool _tasks_distributed = false;
static bool _multicore;
static pid_t pid = 0;

/* Utilised as a drop-in replacement for
   this_core->running_task on non-SMP systems */
static struct task *running_task = &root;

void switch_task(regs_t *r, uint64_t jiffies);

declare_lock(sched_lock);

/**
 * @brief Make a copy of a task (disconnected from its tree)
 */
static struct task *copy_task(struct task *t)
{
    struct task *cp = kmalloc(sizeof(struct task));
    cp->name = t->name;
    cp->pid = t->pid;
    cp->context = t->context;
    cp->state = t->state;
    cp->assigned_to_cpu = t->assigned_to_cpu;
    cp->next = NULL; /* disconnect task */
    cp->children = t->children;
    return cp;
}

/**
 * @brief Append task to CPU
 */
static void copy_task_to_cpu(struct processor *cpu, struct task *t)
{
    /**
     * Since the other CPU receives a copy
     * of this task, the BSP will have to signal
     * other APs when a task's state changes
     */
    struct task *ptr = cpu->root;
    t->assigned_to_cpu = cpu->cpu_id;
    if (!ptr) {
        cpu->root = copy_task(t);
        return;
    }

    while (ptr->next) {
        ptr = ptr->next;
    }

    ptr->next = copy_task(t);
}

/**
 * @brief Perform load balancing
 *
 * This function tries to makes sure that our tasks are
 * distributed evenly & fairly between all CPUs.
 *
 * in order to achieve that, we apply the following algorithm:
 * Let n be the number of CPUs & m the number of tasks. If n is
 * higher than m, we first assign n/m tasks to each CPU. If m
 * is higher than (or equal) to n, we assign m/n tasks to each CPU.
 *
 * In case that the previous step did not result in a perfect
 * division, we loop over all unassigned tasks & we spread them
 * evenly across all CPUs.
 *
 * @param added_task If added_task is NULL, then all tasks will
 * be assigned from root. If added_task is not NULL, the only task
 * that will be considered is the task being pointed to.
 */
static void assign_tasks_to_cpus(struct task *added_task)
{
    struct processor **cores = cpus->cpus;
    struct task *taskptr = &root;
    static unsigned int added_task_idx = 0;
    /* Do we consider all tasks or not? */
    if (added_task == NULL) {
        /* Are there tasks to assign? */
        if (pid > 1) {
            /* Skip root task */
            taskptr = taskptr->next;
            /* If there are more CPUs than tasks,
               we assign CPUs/pid tasks to each CPU. If
               there are more (or equal) tasks than CPUs,
               we assign pid/CPUs to each CPU. */
            unsigned int cpus_ht_pid = cpus->size > pid;
            unsigned int share = cpus_ht_pid ? (cpus->size / pid) : (pid / cpus->size);
            for (size_t i = 0; i < cpus->size; i++) {
                /* Break from here as well if there are no more tasks... */
                if (!taskptr->next) break;
                for (size_t j = 0; j < share; j++) {
                    /* Copy task to CPU */
                    copy_task_to_cpu(cores[i], taskptr);
                    /* Next task! (if a next task exists) */
                    if (taskptr->next) {
                        taskptr = taskptr->next;
                    } else {
                        break;
                    }
                }
            }

            /* Assign leftover tasks */
            taskptr = root.next;
            unsigned int i = 0;
            while (taskptr) {
                /* Found unassigned task */
                if (taskptr->assigned_to_cpu < 0) {
                    copy_task_to_cpu(cores[i++], taskptr);
                    if (i == cpus->size) i = 0;
                }
                taskptr = taskptr->next;
            }
        } else {
            /* May not be the BSP, but who cares?? */
            copy_task_to_cpu(cores[0], taskptr->next);
        }
    } else {
        /* Only add this task! */
        copy_task_to_cpu(cores[added_task_idx++], added_task);
        if (added_task_idx == cpus->size) added_task_idx = 0;
    }
}

/**
 * @brief Initialise task
 *
 * Allocate a context for this task
 */
static void init_task(struct task *t, const char *name)
{
    static struct task *prev = &root;

    /* Fill in all the fields */
    t->pid = ++pid;
    t->name = name;
    t->context = mmu_init_context(PROC_HEAP_SIZE, PROC_STACK_LOW);
    t->state = SUSPENDED;
    t->assigned_to_cpu = -1;
    t->children = list();
    t->next = NULL;

    /* Add task to the linked list */
    prev->next = t;
    t->next = NULL;
    prev = prev->next;

    /* It's probably time to distribute tasks! */
    if (pid > SCHED_SINGLE_CORE_MAX_TASKS && !_tasks_distributed && _multicore) {
        assign_tasks_to_cpus(NULL);
        _tasks_distributed = true;
    } else if (_tasks_distributed && _multicore) {
        /* Assign ONLY new task */
        assign_tasks_to_cpus(t);
    }
}

void sched_run_task(const char *name)
{
    lock(&sched_lock);
    struct task *t = kmalloc(sizeof(struct task));
    init_task(t, name);
    unlock(&sched_lock);
}

void sched_init(void)
{
    /* Check if we run within a multicore system */
    cpus = smp_get_cores();
    _multicore = (cpus->size > 1);
    clock_hook(switch_task);
    pr_info("sched: initialised scheduler (%s multicore)", _multicore ? "with" : "without");
}

/**
 * @brief Save task context & switch
 *
 * Saves t1's context from the CPU to its structure
 * and switches the CPU's context to t2's context
 */
static void save_task_context_and_switch(regs_t *r, struct task *t1, struct task *t2)
{
    if (t2->state == SUSPENDED) {
        /* Suspend current task and run next task */
        t1->state = SUSPENDED;
        t2->state = RUNNING;
        /* Context switch (if t2 isn't root!) */
        if (t2->pid > 0) {
            t1->context->regs = r;
            mmu_switch(t2->context);
        }
    }
}

/**
 * @brief Per processor task switch
 * @todo task children?
 *
 * Checks which CPU calls & switches to its
 * next task
 */
void switch_task(regs_t *r, uint64_t jiffies)
{
    UNUSED(jiffies);
    const bool multicore_and_distributed = _multicore && _tasks_distributed;
    struct task *prev;
    /* If we are in the middle of a sched_run_task call,
       return so that we don't run into a race condition! */
    if (sched_lock) return;

    /* I am aware that this is a very stupid way to go about it and it can
       be reduced to just a few lines, but when I try to do away with
       the duplication by using a pointer, it GPFs! Go figure! */
    if (multicore_and_distributed) {
        prev = this_core->running_task;
        /* Initialise running_task */
        if (!this_core->running_task) {
            this_core->running_task = this_core->root;
        /* Same shit as below */
        } else if (!this_core->running_task->next) {
            save_task_context_and_switch(r, this_core->running_task, this_core->root);
            this_core->running_task = this_core->root;
            return;
        } else {
            this_core->running_task = this_core->running_task->next;
        }
    } else {
        prev = running_task;
        /* Jump from last task in list to root task (does some
           dodgy stuff, but it shouldn't matter...) */
        if (!running_task->next) {
            save_task_context_and_switch(r, running_task, &root);
            running_task = &root;
            return;
        } else {
            running_task = running_task->next;
        }
    }
    /* Switch tasks! */
    if (prev)
        save_task_context_and_switch(r, prev, multicore_and_distributed ? this_core->running_task : running_task);
}
