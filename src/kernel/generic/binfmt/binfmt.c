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
 * Static elf64 loader :^)
 *
 * TODO: Destroying objects??
 */

#include "binfmt.h"

declare_lock(binfmt_lock);
declare_lock(exec_lock);

static struct object_pool *obj_pool = NULL;

void binfmt_init(void)
{
    /* We first initialise a slab capable
     * of holding 1MiB objects (TODO: extend this
     * to arbitrarily sized executables) */
    obj_pool = pool_create("binfmt", NULL, MiB(1));
}

/**
 * @brief Load object
 *
 * Loads ELF object from the filesystem.
 */
static int binfmt_load(const char *path, struct binfmt_object *obj)
{
    struct stat st_buf;
    struct fs_node *f;

    if (!(f = kopen(path, 0))) {
        pr_err("binfmt: couldn't locate object at \"%s\"", path);
        return -ENOENT;
    }

    if (fstat(f, &st_buf) < 0) {
        pr_err("binfmt: cannot stat \"%s\"", path);
        return -ENOENT;
    }

    void *buf = pool_alloc(obj_pool);
    if (fread(f, 0, buf, st_buf.st_size) >= 0) {
        pr_debug("binfmt: loaded %u bytes", st_buf.st_size);
        obj->name = strdup(path);
        obj->buf = buf;
    } else {
        pr_err("binfmt: cannot read from \"%s\"", path);
        return -ENOENT;
    }

    if (kclose(f) < 0) {
        pr_err("binfmt: couldn't close file \"%s\"", path);
        return -ENOENT;
    }

    return ENORM;
}

/**
 * @brief Execute object pointed at by obj
 *
 * Executes object already present in memory
 *
 * @param obj object to be executed
 */
static int _exec(struct binfmt_object *obj)
{
    lock(&exec_lock);
    struct task *t;
    if (!(t = sched_create_task(strdup(obj->name)))) {
        unlock(&exec_lock);
        pr_err("binfmt: cannot exec \"%s\"", obj->name);
        return -ENOENT;
    }

    /* Load the damn thing already! */
    struct elf_stat st;
    elf_load(obj->buf, t->context, &st);

    if (st.loaded) {
        t->context->regs->rip = st.entry;
        sched_queue(t);
        pr_info("exec: scheduled \"%s\"", obj->name);
    } else {
        unlock(&exec_lock);
        pr_err("binfmt: \"%s\" is not a valid executable", obj->name);
        return -ENOEXEC;
    }

    unlock(&exec_lock);
    return ENORM;
}

/**
 * @brief Load executable & schedule it to run
 *
 * @param path Path to executable
 * @param argv Arguments
 * @param envp Environment variables
 */
int exec(const char *path, const char *argv[], const char *envp[])
{
    lock(&binfmt_lock);
    UNUSED(argv); /* TODO */
    UNUSED(envp);

    struct binfmt_object obj;
    if (binfmt_load(path, &obj) < 0) {
        unlock(&binfmt_lock);
        pr_err("exec: cannot load executable at \"%s\"", path);
        return -ENOENT;
    }

    unlock(&binfmt_lock);
    return _exec(&obj);
}
