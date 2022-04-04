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
 * Lower level executable parsing
 */

#include "exec.h"

/**
 * @brief Execute the file located at some path
 *
 * @param path Executable location
 * @param argc Number of arguments
 * @param argv Array of arguments
 */
int exec(const char *path, int argc, const char *argv)
{
    UNUSED(argc);
    UNUSED(argv);

    struct fs_node *file = kopen(path, 0);
    if (!file) {
        pr_err("exec: %s: no such file or directory!", path);
        return -EINVAL;
    }

    /* First, we create a task (maybe strdup this?) */
    struct task *task = sched_create_task(path);

    struct stat st;
    fstat(file, &st);

    /* Just read the whole damn thing into memory, its fine... */
    void *elf = kmalloc(st.st_size);
    struct elf_stat elf_st;
    fread(file, 0, elf, st.st_size);
    elf_load(elf, task->context, &elf_st);

    if (!elf_st.loaded) {
        pr_err("exec: %s: couldn't execute file", path);
        return -EINVAL;
    }

    task->context->regs->rip = elf_st.entry;
    kfree(elf);
    return ENORM;
}
