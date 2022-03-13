
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
 */

#pragma once

#include <stdint.h>

#include <lib/tree.h>
#include <lib/string.h>

#include <generic/malloc/malloc.h>

#define VFS_MAX_FILE_NAME 256

typedef void (*ioctl_func)(int, int);
typedef void (*write_func)(const void *, size_t);
typedef void *(*read_func)(int, size_t);

enum fs_node_flags {
    VFS_IS_DEVICE = (1 << 0),
};

struct vfs_file {
    int permissions;    /* TODO */
    char *name;   /* Limited to 255 */
    /* fs info... */
};

struct fs_node {
    struct vfs_file *file;
    int flags;
    ioctl_func ioctl;
    write_func write;
    read_func  read;
};

struct vfs {
    struct tree *tree;
};

/**
 * vfs_init
 *   brief: initialise vfs
 */
void vfs_init(void);

/**
 * vfs_mount
 *   brief: mount fs node
 */
void vfs_mount(const char *path, struct fs_node *node);
